#pragma once

#include "Common\DeviceResources.h"
#include "Common\StepTimer.h"

#include "Content\SpinningMoleculeRenderer.h"
#include "Content\SpatialInputHandler.h"

// Updates, renders, and presents holographic content using Direct3D.
namespace HoliMoli
{
    class HoliMoliMain : public DX::IDeviceNotify
    {
    public:
        HoliMoliMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
        ~HoliMoliMain();

        // Sets the holographic space. This is our closest analogue to setting a new window
        // for the app.
        void SetHolographicSpace(Windows::Graphics::Holographic::HolographicSpace^ holographicSpace);

        // Creates a list of speech commands for the recognizer to listen for. 
        void CreateSpeechConstraintsForCurrentState();
        void ReleaseSpeechConstraintsForCurrentState();
        void OnSpeechQualityDegraded(Windows::Media::SpeechRecognition::SpeechRecognizer^ recognizer,
                                     Windows::Media::SpeechRecognition::SpeechRecognitionQualityDegradingEventArgs^ args);
        void OnResultGenerated(Windows::Media::SpeechRecognition::SpeechContinuousRecognitionSession^ sender,
                               Windows::Media::SpeechRecognition::SpeechContinuousRecognitionResultGeneratedEventArgs^ args);

        // Starts the holographic frame and updates the content.
        Windows::Graphics::Holographic::HolographicFrame^ Update();

        // Renders holograms, including world-locked content.
        bool Render(Windows::Graphics::Holographic::HolographicFrame^ holographicFrame);

        // Handle saving and loading of app state owned by AppMain.
        void SaveAppState();
        void LoadAppState();

        // IDeviceNotify
        virtual void OnDeviceLost();
        virtual void OnDeviceRestored();

    private:
        // Asynchronously creates resources for new holographic cameras.
        void OnCameraAdded(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraAddedEventArgs^ args);

        // Synchronously releases resources for holographic cameras that are no longer
        // attached to the system.
        void OnCameraRemoved(
            Windows::Graphics::Holographic::HolographicSpace^ sender,
            Windows::Graphics::Holographic::HolographicSpaceCameraRemovedEventArgs^ args);

        // Used to notify the app when the positional tracking state changes.
        void OnLocatabilityChanged(
            Windows::Perception::Spatial::SpatialLocator^ sender,
            Platform::Object^ args);

        // Clears event registration state. Used when changing to a new HolographicSpace
        // and when tearing down AppMain.
        void UnregisterHolographicEventHandlers();

        // Renders a colorful holographic molecule that's 20 centimeters wide. This sample content
        // is used to demonstrate world-locked rendering.
        std::unique_ptr<SpinningMoleculeRenderer>                        m_spinningMoleculeRenderer;

        // Listens for the Pressed spatial input event.
        std::shared_ptr<SpatialInputHandler>                             m_spatialInputHandler;

        // Cached pointer to device resources.
        std::shared_ptr<DX::DeviceResources>                             m_deviceResources;

        // Render loop timer.
        DX::StepTimer                                                    m_timer;

        // Represents the holographic space around the user.
        Windows::Graphics::Holographic::HolographicSpace^                m_holographicSpace;

        // SpatialLocator that is attached to the primary camera.
        Windows::Perception::Spatial::SpatialLocator^                    m_locator;

        // A reference frame attached to the holographic camera.
        Windows::Perception::Spatial::SpatialStationaryFrameOfReference^ m_referenceFrame;

        // Event registration tokens.
        Windows::Foundation::EventRegistrationToken                      m_cameraAddedToken;
        Windows::Foundation::EventRegistrationToken                      m_cameraRemovedToken;
        Windows::Foundation::EventRegistrationToken                      m_locatabilityChangedToken;

        // Sound related stuff.
        std::unique_ptr<DirectX::AudioEngine>                            m_audioEngine;
        std::unique_ptr<DirectX::WaveBank>                               m_waveBank;

        // Voice input stuff.
        Windows::Media::SpeechRecognition::SpeechRecognizer^             m_speechRecognizer;
        Platform::Collections::Vector<Platform::String^>^                m_speechCommandList;
        bool                                                             m_repositionMolecule = false;
    };
}
