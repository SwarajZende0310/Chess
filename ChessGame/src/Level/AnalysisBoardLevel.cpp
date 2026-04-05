/**
 * @file AnalysisBoardLevel.cpp
 * @brief Implementation of the analysis board level: HUD wiring, rendering, and input forwarding.
 */
#include"Level/AnalysisBoardLevel.h"
#include"Level/MainMenuLevel.h"
#include"framework/Application.h"
#include"framework/ChessState.h"
#include"widgets/AnalysisBoardHUD.h"

namespace chess
{
    /**
     * @brief Construct the analysis level with the owning application context.
     */
    AnalysisBoardLevel::AnalysisBoardLevel(Application *owningApp)
        :Stage{owningApp},
        mEngine{std::make_unique<BacktrackingEngine>(10, 20000000, 1500)},
        mAnalysisWorker{},
        mAnalysisMutex{},
        mAnalysisCv{},
        mStopWorker{false},
        mCancelSearch{false},
        mQueuedGeneration{0},
        mActiveGeneration{0},
        mLastPublishedGeneration{0},
        mHasQueuedRequest{false},
        mHasCompletedAnalysis{false},
        mQueuedFen{},
        mCompletedAnalysis{}
    {
        mAnalysisWorker = std::thread(&AnalysisBoardLevel::AnalysisWorkerLoop, this);
    }

    AnalysisBoardLevel::~AnalysisBoardLevel()
    {
        mStopWorker.store(true);
        mCancelSearch.store(true);
        mAnalysisCv.notify_all();
        if(mAnalysisWorker.joinable())
        {
            mAnalysisWorker.join();
        }
    }

    /**
     * @brief Spawn the analysis HUD and bind its button delegates.
     */
    void AnalysisBoardLevel::BeginPlay()
    {
        mAnalysisBoardHUD = SpawnHUD<AnalysisBoardHUD>();

        mAnalysisBoardHUD.lock()->onHomeButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::GoHome);
        mAnalysisBoardHUD.lock()->onQuitButtonClicked.BindAction(GetWeakRef(), &AnalysisBoardLevel::EndGame);
        mOnEvaluationUpdate.BindAction(mAnalysisBoardHUD, &AnalysisBoardHUD::UpdateCurrentEvaluation);
        RefreshEvaluationFromCurrentPosition();
    }

    /**
     * @brief Render the board, pieces, and HUD each frame.
     */
    void AnalysisBoardLevel::Render()
    {
        RenderBoard();
        RenderPieces();
        RenderHUD(GetApplication()->GetWindow());
    }

    /**
     * @brief Forward events to the board handler for piece interactions.
     * @return true if the event was consumed by the board logic.
     */
    bool AnalysisBoardLevel::HandleEventInternal(const std::optional<sf::Event> & event)
    {
        const bool handled = HandleBoardEvent(event);
        if(IsPieceMoved())
        {
            RefreshEvaluationFromCurrentPosition();
        }
        return handled;
    }

    /**
     * @brief Navigate back to the main menu level.
     */
    void AnalysisBoardLevel::GoHome()
    {
        GetApplication()->LoadWorld<MainMenuLevel>();
    }

    /**
     * @brief Quit the application from the analysis HUD.
     */
    void AnalysisBoardLevel::EndGame()
    {
        GetApplication()->QuitApplication();
    }

    void AnalysisBoardLevel::RefreshEvaluationFromCurrentPosition()
    {
        std::lock_guard<std::mutex> lock(mAnalysisMutex);
        mQueuedFen = ChessState::Get().GetCurrentPositionInFEN(IsWhiteTurn());
        ++mQueuedGeneration;
        mHasQueuedRequest = true;
        mCancelSearch.store(true);
        mAnalysisCv.notify_one();
    }

    void AnalysisBoardLevel::CalculateCurrentEvaluation()
    {
        CompletedAnalysis completedAnalysis;
        bool hasCompletedAnalysis = false;

        {
            std::lock_guard<std::mutex> lock(mAnalysisMutex);
            if(mHasCompletedAnalysis && mCompletedAnalysis.generation > mLastPublishedGeneration)
            {
                completedAnalysis = mCompletedAnalysis;
                mLastPublishedGeneration = completedAnalysis.generation;
                hasCompletedAnalysis = true;
            }
        }

        if(hasCompletedAnalysis)
        {
            LOG("AnalysisBoard best move: %s | eval: %.1f | time: %lld ms | nodes: %zu | depth: %d | aborted: %s",
                completedAnalysis.result.bestMove.c_str(),
                completedAnalysis.result.evaluation,
                completedAnalysis.result.elapsedMs,
                completedAnalysis.result.visitedNodes,
                completedAnalysis.result.completedDepth,
                completedAnalysis.result.aborted ? "true" : "false");
            PublishCurrentEvaluation(completedAnalysis.result.evaluation);
        }
    }

    void AnalysisBoardLevel::AnalysisWorkerLoop()
    {
        while(true)
        {
            std::string fen;
            uint64_t generation = 0;

            {
                std::unique_lock<std::mutex> lock(mAnalysisMutex);
                mAnalysisCv.wait(lock, [this]()
                {
                    return mStopWorker.load() || mHasQueuedRequest;
                });

                if(mStopWorker.load())
                {
                    return;
                }

                generation = mQueuedGeneration;
                fen = mQueuedFen;
                mActiveGeneration = generation;
                mHasQueuedRequest = false;
                mCancelSearch.store(false);
            }

            EngineSearchRequest request;
            request.fen = fen;
            request.cancelFlag = &mCancelSearch;

            EngineSearchResult result = mEngine->AnalyzePosition(request);

            {
                std::lock_guard<std::mutex> lock(mAnalysisMutex);
                if(mStopWorker.load())
                {
                    return;
                }

                if(generation < mQueuedGeneration)
                {
                    continue;
                }

                mCompletedAnalysis.generation = generation;
                mCompletedAnalysis.result = std::move(result);
                mHasCompletedAnalysis = true;
            }
        }
    }
}
