# Bugs Found During Manual Testing

This file documents the bugs found during the manual testing phase of the project.

## 1. Program window does not update after transition

*   **Steps to reproduce:**
    1.  Add a source to a scene.
    2.  Select the source in the preview window.
    3.  Click "Transition".
*   **Expected behavior:**
    The program window should display the live feed of the transitioned source.
*   **Actual behavior:**
    The program window displays only the first frame of the transitioned source. It does not update with new frames.

## 2. Drag-and-drop source positioning is not working correctly

*   **Steps to reproduce:**
    1.  Add a source to a scene.
    2.  Select the source in the preview window.
    3.  Drag the source to a new position.
*   **Expected behavior:**
    The source should move to the new position in the preview window, and the compositor should update the source's position in the program window.
*   **Actual behavior:**
    The source moves in the preview window, but the compositor does not update the source's position. The source remains in its original position in the program window.

## 3. Audio controls are not working

*   **Steps to reproduce:**
    1.  Add a source with audio to a scene.
    2.  Use the volume slider to change the volume.
    3.  Click the mute button.
*   **Expected behavior:**
    The volume should change, and the audio should be muted.
*   **Actual behavior:**
    The volume and mute controls have no effect on the audio.
