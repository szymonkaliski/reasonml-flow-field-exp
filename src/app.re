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

let regl = Regl.make canvas;

let triangleAttributes = [%bs.obj {
  position: [|
    [| -0.75, -0.5 |],
    [| 0.75, -0.5 |],
    [| 0., 0.5 |]
  |]
}];

let drawTriangleConfig = Regl.makeDrawConfig
  frag:: "
    precision mediump float;
    void main() {
      gl_FragColor = vec4(0.3, 0.8, 0.9, 1.);
    }
  "
  vert:: "
    precision mediump float;
    attribute vec2 position;
    void main() {
      gl_Position = vec4(position, 0., 1.);
    }
  "
  attributes:: triangleAttributes
  uniforms:: ()
  count:: 3;

let drawTriangleComand = Regl.makeDrawCommand regl drawTriangleConfig;

Regl.frame regl (fun () => {
  Regl.drawCommand drawTriangleComand;
});
