import React from 'react';

const AudioMixer = () => {
  return (
    <div>
      <h2>Audio Mixer</h2>
      {/* Placeholder for audio controls */}
      <div>
        <span>Mic:</span>
        <input type="range" />
      </div>
      <div>
        <span>Desktop:</span>
        <input type="range" />
      </div>
    </div>
  );
};

export default AudioMixer;
