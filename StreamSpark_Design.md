# StreamSpark: Application Design Document

## 1. Vision & Strategy

### 1.1. Application Name

**StreamSpark**

### 1.2. Vision

To empower social media creators with a simple, intuitive, and powerful tool to produce professional-quality live streams. StreamSpark bridges the gap between complex broadcast software and basic "go live" buttons on social platforms, making engaging content creation accessible to everyone.

### 1.3. Target Audience

The primary target audience is the everyday **social media user and emerging creator**. This user is familiar with platforms like Twitch, YouTube Live, and Instagram Live but may be intimidated by technical software like OBS or XSplit. They want to enhance their streams with simple branding, screen sharing, and basic interactivity without a steep learning curve.

## 2. Core Functionality

### 2.1. Virtual Media Engine

#### 2.1.1. Virtual Camera

The core of the application is a virtual camera that the user's system and other applications (e.g., web browsers for streaming platforms) can select as a video source.

*   **Scene-Based Composition:** Users can create and switch between multiple "scenes" (e.g., "Starting Soon," "Main Scene," "Be Right Back").
*   **Video Sources:**
    *   Webcam feed (with resolution and frame rate selection).
    *   Screen share (full display, specific application window, or a defined region).
    *   Image overlays (e.g., logos, watermarks, alert graphics). Supports PNG with transparency.
    *   Text overlays (static or dynamically updated from files).
*   **Simple Layering:** Sources within a scene are layered and can be easily re-ordered (bring to front, send to back).

#### 2.1.2. Virtual Audio Mixer

A corresponding virtual audio device that mixes multiple audio inputs into a single output.

*   **Audio Sources:**
    *   Microphone input (with a simple noise gate/suppression filter).
    *   Desktop/Application audio capture (e.g., for game sounds or music).
    *   Soundboard: A simple grid for playing pre-loaded sound effects (e.g., applause, alerts).
*   **Mixer Controls:** A user-friendly mixer with volume sliders and mute toggles for each audio source.

### 2.2. Platform Integration

#### 2.2.1. Social Media Authentication

*   Secure OAuth 2.0 integration for one-click login and connection with major platforms (YouTube, Twitch, Facebook Live).
*   The application securely stores API tokens locally and manages stream keys automatically, abstracting the complexity from the user.

#### 2.2.2. Live Chat Integration

*   A dedicated panel within the app to display the live chat from the connected streaming platform.
*   **"Feature Comment" feature:** A single button next to each chat message that, when clicked, displays the comment as a stylized overlay on the stream for a few seconds. This is a key user interaction element.

### 2.3. Streaming Management

*   **"Go Live" Hub:** A central area in the UI to set the stream title and description, select the destination platform, and start/stop the stream with a single, prominent button.
*   **Stream Health Monitoring:** Simple, color-coded indicators for CPU usage and network connection quality to prevent dropped frames.

## 3. User Experience (UX) Flow

### 3.1. Onboarding

1.  **Welcome Screen:** On first launch, the user is greeted with a brief tour of the three main UI sections: Scenes/Sources, Canvas, and Stream Control.
2.  **Device Setup:** The app automatically detects webcams and microphones, prompting the user for permission.
3.  **Account Linking:** The user is prompted to connect their primary streaming account (e.g., YouTube). This step is optional but recommended for the full experience.

### 3.2. Pre-Live Workflow

1.  **Scene Creation:** The user starts with a default "Main" scene. They can add sources like their webcam and a logo.
2.  **Canvas Interaction:** The user arranges sources by clicking and dragging them on the main preview canvas. Resizing is done by dragging the corners of the selected source.
3.  **Audio Check:** The user checks the audio mixer to ensure their microphone is active and at the correct volume.
4.  **Stream Setup:** The user types in their stream title in the "Stream Control" panel.

### 3.3. Live Streaming Workflow

1.  **Initiate Stream:** The user clicks the large "Go Live" button. A 3-second countdown appears on the preview, and the button changes to a red "End Stream" button, displaying the stream duration.
2.  **Interaction:** During the stream, the user can:
    *   Switch between scenes (e.g., switch to a "Be Right Back" scene).
    *   Monitor the live chat feed.
    *   Click the "Feature" button on a user's comment to display it on screen.
    *   Trigger sound effects from the soundboard.
3.  **End Stream:** The user clicks the "End Stream" button. A confirmation dialog appears. Once confirmed, the stream is terminated. A brief summary (duration, final viewer count) is displayed.

## 4. User Interface (UI) Sketches (Textual Description)

The application will use a clean, modern, and dark-themed UI to minimize eye strain and focus attention on the video content.

### 4.1. Main Application Window

A three-column layout provides a logical and intuitive workflow.

```
+---------------------------------------------------------------------------------+
| [File] [Edit] [View] [Help]                                                     |
+--------------------------------+----------------------+-------------------------+
|                                |                      |                         |
|  COLUMN 1: SCENES & SOURCES    |  COLUMN 2: CANVAS    | COLUMN 3: CONTROL PANEL |
|                                |      (Preview)       |                         |
|  +------------------------+    |                      | +---------------------+ |
|  | Scenes:                |    |                      | | Platform: [YouTube] | |
|  | * Main Scene (Active)  |    |                      | | Title: [My Live...] | |
|  | * BRB Scene            |    |                      | |                     | |
|  |                        |    |  [Live video         | | [   GO LIVE   ]     | |
|  | [+] Add Scene          |    |   preview of the     | |                     | |
|  +------------------------+    |   composed scene]    | +---------------------+ |
|  |                        |    |                      | | LIVE CHAT           | |
|  | Sources for Main Scene:|    |                      | +---------------------+ |
|  | * Webcam (Video)       |    |                      | | User1: Hello!  [F] | |
|  | * Logo (Image)         |    |                      | | User2: Great...[F] | |
|  | * Screen Share (App)   |    |                      | | User3: Can you...[F] | |
|  |                        |    |                      | | ...                 | |
|  | [+] Add Source         |    |                      | +---------------------+ |
|  +------------------------+    |                      |                         |
|                                |                      |                         |
+--------------------------------+----------------------+-------------------------+
| AUDIO MIXER                                                                     |
| [Mic:  |||||||||--] [Desktop: |||||------] [Sounds: ||---------]               |
+---------------------------------------------------------------------------------+
```

*   **Column 1 (Left): Scenes & Sources:**
    *   Top pane lists all created scenes. The active scene is highlighted.
    *   Bottom pane lists the sources for the currently selected scene.
    *   Simple `+` buttons allow for adding new scenes and sources from a dropdown menu.
*   **Column 2 (Center): Canvas:**
    *   The largest part of the UI. It shows a real-time preview of the final video output.
    *   Sources are rendered here and can be directly manipulated (dragged, resized).
*   **Column 3 (Right): Control Panel:**
    *   Top section contains the stream destination, title/description fields, and the main "Go Live" / "End Stream" button.
    *   Bottom section is dedicated to the live chat feed once the stream is active. Each message has a "Feature" (`[F]`) button.
*   **Bottom Bar: Audio Mixer:**
    *   A horizontal bar at the bottom showing VU meters and volume sliders for all active audio sources.

## 5. Technical & Architectural Considerations

### 5.1. Platform & Technology Stack

*   **Desktop App (macOS, Windows):**
    *   **Framework:** **Tauri** or **Electron**. Tauri is preferred for its smaller bundle size and Rust-based backend, offering better performance and security. The UI will be built with **React/TypeScript**.
    *   **Virtual Device Drivers:** The app installer will include drivers to create the "StreamSpark Camera" and "StreamSpark Microphone" devices on the system.
*   **Web App:**
    *   The "web" platform will function primarily as a **dashboard or remote control** for the desktop app. Direct browser APIs for virtual devices (`navigator.mediaDevices`) are not yet mature enough to provide the required functionality without a companion desktop agent.
    *   Users could log into the web app to view chat, get a stream preview, and potentially trigger scene changes, all communicating with their local desktop app via a secure WebSocket connection.

### 5.2. Scalability

*   **Client-Side Processing:** The core video/audio composition and encoding happens on the user's machine. This means the application's "scalability" is tied to the user's hardware, not a central server, making it horizontally scalable by nature.
*   **API Usage:** Backend services (for authentication, etc.) will be designed as serverless functions (e.g., AWS Lambda) to handle authentication traffic spikes and ensure low operational cost. Chat integration relies on the highly scalable APIs of the target platforms.

### 5.3. Cross-Platform Compatibility

*   Using a web-technology-based framework like Tauri/Electron ensures that the core UI and business logic are shared across macOS and Windows.
*   Platform-specific code will be isolated to the virtual device driver installation and management.

## 6. Data Privacy & Compliance

*   **Local-First Data:** All sensitive data, especially OAuth tokens for social media platforms, will be stored encrypted on the user's local machine using the operating system's native keychain/credential manager.
*   **Minimal Permissions:** The app will only request the minimum API scopes required for its functionality (e.g., `youtube.live.broadcasts`, `chat:read`).
*   **GDPR/CCPA:** A clear, easy-to-understand privacy policy will be presented to the user at signup and will be accessible from within the app. The policy will state what little data is collected (e.g., anonymized usage analytics) and how it is used. All data processing will be opt-in.
