# User Interface (UI) Design Principles: live-app

The user interface is a critical component for the success of **live-app**. It must be intuitive for users who may not be video production experts while remaining powerful enough for professional streamers. The design will adhere to the following principles.

## 1. Clarity and Simplicity

*   **Goal:** Users should be able to launch the app, add a source, and start streaming with minimal guidance.
*   **Implementation:**
    *   **Logical Layout:** The UI will be divided into clear, logical sections: Sources, Scenes, Preview, Program, and Controls.
    *   **Minimalism:** Avoid clutter. Only essential controls will be visible by default. Advanced options will be accessible through context menus or settings panels.
    *   **Clear Labeling:** Use universally understood icons and concise text labels. For example, a "▶" icon for "Go Live."

## 2. Standard Broadcast Workflow

*   **Goal:** Adopt a familiar layout for users coming from other broadcasting software.
*   **Implementation:**
    *   **Preview/Program Model:** The UI will feature two main video windows:
        *   **Preview:** Shows the scene that is currently being edited. This is the "on-deck" or "next up" view.
        *   **Program:** Shows the scene that is currently live and being sent to the stream/recording. This is the "live" view.
    *   This model prevents accidental changes from going live and is the standard in professional broadcasting. A clear transition control (e.g., a "Fade" or "Cut" button) will move the content from Preview to Program.

## 3. Direct Manipulation and Feedback

*   **Goal:** Make scene composition feel tangible and intuitive.
*   **Implementation:**
    *   **Drag-and-Drop:** Users will be able to resize and reposition video sources directly within the Preview window.
    *   **Visual Feedback:** Selected sources will have a visible bounding box with handles for resizing. The audio mixer will show real-time volume levels (VU meters).
    *   **Live Indicators:** The "Program" window and "Go Live" button will have a clear, visible "LIVE" indicator with a running timer when the stream is active.

## 4. Consistency

*   **Goal:** Ensure that controls and actions behave predictably throughout the application.
*   **Implementation:**
    *   **Consistent Controls:** Buttons, icons, and menus for similar actions (e.g., "Add," "Remove," "Properties") will look and function the same way across different panels (Sources, Scenes).
    *   ** predictable Layout:** The core layout of the application will remain consistent, providing a stable frame of reference for the user.

## 5. Accessibility for the Target Audience

*   **Goal:** Cater to the specific needs of educators, streamers, and corporate users.
*   **Implementation:**
    *   **For Educators:** The process of adding a webcam and a screen share should be extremely simple, perhaps even a "Quick Start" wizard.
    *   **For Streamers:** Provide easy access to stream keys and status information (e.g., bitrate, dropped frames).
    *   **For Corporate Users:** A clean, professional aesthetic will be prioritized. The ability to easily add a company logo as an overlay is a key feature.

## Mockup / Wireframe Concept

A simple textual wireframe of the main window:

```
+--------------------------------------------------------------------------+
| live-app                                                                 |
+--------------------------------------------------------------------------+
| File  Edit  View  Help                                                   |
+---------------------------------+----------------------------------------+
| PREVIEW                         | PROGRAM (LIVE)                         |
|                                 |                                        |
| (Scene: "Intro")                | (Scene: "Main")                        |
|                                 |                                        |
| [Webcam Feed]                   | [Screen Share with Webcam PIP]         |
|                                 |                                        |
|                                 |                                        |
+---------------------------------+----------------------------------------+
| Scene Transition: [Fade] [Cut]  |                                        |
+---------------------------------+----------------------------------------+
| Scenes     | Sources (in Scene "Intro")  | Audio Mixer                    |
|------------+-----------------------------+--------------------------------|
| * Intro    | - Webcam 1                  | Webcam 1: [|||||    ] Mute    |
| - Main     | - Background Image          | Desktop Audio: [|||||||  ] Mute    |
| - Outro    |                             | Master: [|||||||||] Mute    |
| [+ Add]    | [+ Add] [- Remove]          |                                |
+------------+-----------------------------+--------------------------------+
|                                         | [Start Streaming] [Start Rec]  |
+--------------------------------------------------------------------------+
```
