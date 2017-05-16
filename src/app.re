let canvas = Document.createElement "canvas";

Document.appendChild canvas;

let setCanvasSize _ => {
  let width = Document.getWidth Document.window;
  let height = Document.getHeight Document.window;

  Document.setWidth canvas width;
  Document.setHeight canvas height;
};

Document.addEventListener Document.window "resize" setCanvasSize;
Document.addEventListener Document.window "DOMContentLoaded" setCanvasSize;

Regl.create canvas;

let drawTriangleConfig = Regl.makeDrawConfig
  frag:: "
    precision mediump float;
    uniform vec3 color;
    void main() {
      gl_FragColor = vec4(color, 1.);
    }
  "
  vert:: "
    precision mediump float;
    attribute vec2 position;
    void main() {
      gl_Position = vec4(position, 0., 1.);
    }
  "
  attributes:: [%bs.obj {
    position: [|
      [| -1., -1. |],
      [| 1., 1. |],
      [| -1., 1. |]
    |]
  }]
  uniforms:: [%bs.obj {
    color: Regl.prop "color"
  }]
  count:: 3;

let drawTriangleComand = Regl.makeDrawCommand drawTriangleConfig;

type triangleUniformsT = {
  color: array float
};

let draw t => {
  Regl.clear
    color::[| 0, 0, 0, 1 |]
    depth::1;

  let r = (sin t) *. 0.1;

  let triangleUniforms = Obj.magic [%bs.obj {
    color: [| r, 0.9, 0.8 |]
  }];

  Regl.draw
    command::drawTriangleComand
    uniforms::triangleUniforms;
};

Regl.frame draw;
