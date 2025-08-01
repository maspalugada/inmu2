import React from 'react';

const Canvas = () => {
  return (
    <div>
      <h2>Canvas (Preview)</h2>
      <div style={{
        backgroundColor: '#000',
        width: '100%',
        height: '400px',
        border: '1px solid #444',
        display: 'flex',
        alignItems: 'center',
        justifyContent: 'center'
      }}>
        <p>Video Preview Area</p>
      </div>
    </div>
  );
};

export default Canvas;
