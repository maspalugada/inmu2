import React from 'react';
import './App.css';
import SceneList from './components/SceneList';
import Canvas from './components/Canvas';
import ControlPanel from './components/ControlPanel';
import AudioMixer from './components/AudioMixer';

function App() {
  return (
    <div className="app-container">
      <div className="panels-container">
        <div className="left-panel">
          <SceneList />
        </div>
        <div className="center-panel">
          <Canvas />
        </div>
        <div className="right-panel">
          <ControlPanel />
        </div>
      </div>
      <div className="bottom-panel">
        <AudioMixer />
      </div>
    </div>
  );
}

export default App;
