# Technology Stack: live-app

This document proposes the technology stack for **live-app**, a desktop application for Windows. The choices prioritize performance, modern development practices, and the ability to leverage existing libraries for core multimedia functionalities.

## 1. Core Application Framework

*   **Language: C++ (with C# for UI)**
    *   **Reasoning:** C++ is the industry standard for high-performance multimedia applications, offering direct control over system resources (CPU/GPU) which is critical for video processing and encoding. We will use C# for the UI to enable faster development and a modern look and feel.

*   **Primary Framework: .NET with WPF (Windows Presentation Foundation)**
    *   **Reasoning:** WPF is a powerful UI framework for creating modern and responsive user interfaces on Windows. It separates UI (XAML) from logic (C#), which is great for maintainability. We can use C++/CLI (Common Language Infrastructure) to create a bridge between the C#/.NET frontend and the C++ backend.

## 2. Multimedia and Graphics Backend

*   **Graphics API: Direct3D 11 / DirectX**
    *   **Reasoning:** As a Windows-native application, using DirectX is the most direct and performant way to handle video rendering, composition, and effects on the GPU. This is crucial for offloading CPU-intensive tasks.

*   **Video and Audio Processing Library: GStreamer**
    *   **Reasoning:** GStreamer is a powerful, open-source multimedia framework that handles a wide range of tasks out-of-the-box, including:
        *   **Device Capture:** Interfacing with webcams and audio devices.
        *   **File Playback:** Decoding various video and audio codecs.
        *   **Processing:** Mixing, scaling, and composing video streams.
        *   **Encoding:** Preparing the video for streaming (H.264).
        *   **Streaming:** Pushing the final output to an RTMP server.
    *   Using GStreamer will significantly accelerate development by providing pre-built, optimized components for the most complex parts of the application.

## 3. Streaming Protocol

*   **Protocol: RTMP (Real-Time Messaging Protocol)**
    *   **Reasoning:** RTMP is the de facto standard for streaming to major platforms like YouTube, Twitch, and Facebook. GStreamer has excellent support for RTMP sinks.

## 4. Development and Build Tools

*   **IDE: Visual Studio 2022**
    *   **Reasoning:** Provides an all-in-one environment for C#, C++, and .NET development, with excellent debugging and profiling tools.

*   **Build System: MSBuild (via Visual Studio)**
    *   **Reasoning:** The standard build system for .NET and C++ projects on Windows.

*   **Dependency Management:**
    *   **NuGet:** For .NET/C# libraries.
    *   **Vcpkg or Conan:** For managing C++ dependencies like GStreamer. This simplifies the process of acquiring and linking third-party libraries.

## Architecture Summary

The application will have a two-part architecture:

1.  **UI Layer (C# / WPF):**
    *   Manages all user interactions, windows, and controls.
    *   Sends commands to the backend (e.g., "add webcam," "start stream").
    *   Renders the final video output, which is passed from the backend as a shared DirectX texture.

2.  **Core Engine (C++ / GStreamer):**
    *   Handles all heavy lifting: video/audio capture, processing, mixing, and encoding.
    *   Is controlled by the UI layer via a C++/CLI wrapper.
    *   Runs its processing pipeline and makes the final rendered video frame available to the UI for display in the "Program" window.

This hybrid approach combines the rapid UI development of C#/.NET with the high performance of C++/GStreamer, which is ideal for a real-time video application.
