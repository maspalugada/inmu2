# Feature List: live-app (Version 1.0)

This document outlines the core features for the initial release of **live-app**. The focus is on providing a stable and functional foundation that meets the primary needs of our target audience.

## 1. Video Input Sources

The application will support a variety of input sources, which can be added and managed in a scene.

*   **Webcams & Capture Cards:**
    *   Auto-detect connected USB webcams and other video devices.
    *   Allow users to select resolution and frame rate.
*   **Screen Capture:**
    *   Capture the entire screen or a specific application window.
    *   Option to include/exclude the cursor.
*   **Video Files:**
    *   Support for common video formats (e.g., MP4, MOV, AVI).
    *   Playback controls (play, pause, loop).
*   **Image Files:**
    *   Support for PNG, JPG, and other common image formats for use as overlays or full-screen graphics.

## 2. Audio Mixing

*   **Input Management:**
    *   Capture audio from video sources (e.g., webcam microphone).
    *   Independent volume control for each audio source.
    *   Master output volume control.
*   **Mute/Solo:**
    *   Ability to mute individual audio sources.

## 3. Scene and Source Management

*   **Scenes:**
    *   Create multiple scenes to prepare different layouts.
    *   Switch between scenes with a single click.
*   **Layers (Sources):**
    *   Add multiple sources to a single scene.
    *   Resize, position, and layer sources via a drag-and-drop interface.

## 4. Transitions

*   **Basic Scene Transitions:**
    *   **Cut:** An instantaneous switch between scenes.
    *   **Fade:** A gradual fade-to-black and then fade-in to the next scene.
    *   Configurable transition duration.

## 5. Overlays and Graphics

*   **Text Overlay:**
    *   Add simple text elements to the scene.
    *   Customize font, size, color, and position.
*   **Image Overlay:**
    *   Add logos or other graphics (e.g., PNG with transparency) on top of the video.

## 6. Streaming and Recording

*   **Live Streaming:**
    *   **YouTube Integration:** Simple setup process to stream directly to a user's YouTube channel (using Stream Keys).
    *   **Custom RTMP:** Support for streaming to any platform via a custom RTMP server URL and stream key.
*   **Local Recording:**
    *   Record the final mixed output to a local file (MP4 format).
    *   User-configurable recording quality and save location.

## 7. User Interface

*   **Main Layout:**
    *   **Preview Window:** Shows the currently selected scene before it goes live.
    *   **Program (Live) Window:** Shows the final output that is being streamed/recorded.
    *   **Source List:** A panel to add and manage input sources.
    *   **Scene List:** A panel to create and switch between scenes.
    *   **Audio Mixer:** A panel to control audio levels.
    *   **Stream/Record Controls:** Clear, accessible buttons to start and stop streaming/recording.
