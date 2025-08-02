# Development Timeline: live-app (Version 1.0)

This document provides a high-level, estimated timeline for the development of **live-app** version 1.0. The timeline is broken down into key phases and assumes a small, focused development team.

---

### **Phase 1: Project Setup & Core Backend (Weeks 1-3)**

*   **Goal:** Establish the foundational architecture and get a basic video pipeline working.
*   **Tasks:**
    *   Set up the Visual Studio solution with C# (WPF) and C++ projects.
    *   Integrate GStreamer and DirectX dependencies.
    *   Create the C++/CLI bridge between the C# UI and C++ core.
    *   **Milestone:** Implement a proof-of-concept that can capture a webcam feed using GStreamer and display it in a simple WPF window.

---

### **Phase 2: Core Feature Development (Weeks 4-8)**

*   **Goal:** Implement the primary multimedia functionalities.
*   **Tasks:**
    *   **Input Management (Weeks 4-5):**
        *   Implement robust webcam and capture card input.
        *   Add screen and application capture functionality.
        *   Implement video file playback.
    *   **Scene Composition (Week 6):**
        *   Develop the scene and source management system.
        *   Implement resizing, positioning, and layering of sources on a DirectX surface.
    *   **Audio Mixing (Week 7):**
        *   Implement basic audio capture and mixing for all input sources.
        *   Add volume controls and mute functionality.
    *   **Streaming & Recording (Week 8):**
        *   Implement the RTMP streaming pipeline to YouTube.
        *   Implement local recording to an MP4 file.
    *   **Milestone:** A functional backend that can mix multiple sources and stream the output.

---

### **Phase 3: UI/UX Development (Weeks 9-12)**

*   **Goal:** Build the user-facing interface based on the UI design principles.
*   **Tasks:**
    *   **Main Window Layout (Week 9):**
        *   Develop the main application window with Preview/Program views, and panels for scenes, sources, and audio.
    *   **Interaction (Weeks 10-11):**
        *   Implement drag-and-drop for source manipulation.
        *   Connect all UI controls (buttons, sliders) to the backend core engine functions.
        *   Implement scene switching with "Cut" and "Fade" transitions.
    *   **Overlays (Week 12):**
        *   Implement UI for adding and customizing text and image overlays.
    *   **Milestone:** A fully interactive UI that controls all backend features. The application is now feature-complete.

---

### **Phase 4: Testing & Bug Fixing (Weeks 13-14)**

*   **Goal:** Ensure the application is stable, performant, and ready for users.
*   **Tasks:**
    *   **Internal Testing:**
        *   Test all features across different hardware configurations.
        *   Identify and fix bugs related to performance, UI, and functionality.
        *   Test streaming stability over extended periods.
    *   **User Acceptance Testing (UAT):**
        *   (Optional but recommended) Share a beta version with a small group of target users (e.g., a friendly streamer or educator) to gather feedback.
    *   **Milestone:** A stable beta version of the application.

---

### **Phase 5: Finalization & Deployment (Week 15)**

*   **Goal:** Package the application and prepare for release.
*   **Tasks:**
    *   Create a user-friendly installer (e.g., using Inno Setup or MSIX).
    *   Write basic user documentation or a quick-start guide.
    *   Prepare the project website or repository for public release.
    *   **Milestone:** Version 1.0 of **live-app** is ready for download.

---

### **Total Estimated Timeline: 15 Weeks**

**Disclaimer:** This is an estimate. The timeline can be affected by unforeseen technical challenges, changes in scope, or resource availability. Regular progress reviews will be necessary to adjust the plan as needed.
