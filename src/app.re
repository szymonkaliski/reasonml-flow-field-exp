let canvas = Document.createElement "canvas";
let ctx = Canvas.getContext canvas "3d";

Document.appendChild canvas;

let regl = Regl.makeRegl ctx;

let triangleAttributes = [%bs.obj {
  position: [|
    [| -2, -2 |],
    [| 4, -2 |],
    [| 4, 4 |]
  |]
}];

let drawTriangleConfig = Regl.makeDrawConfig
  frag:: "
    precision mediump float;
    void main() {
      gl_FragColor = vec4(1., 0., 0., 1.);
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

let drawTriangleComand = regl drawTriangleConfig;


let draw () => {
  /* TODO: uncurry? */
  [%bs.raw "drawTriangleComand()"];
};

Regl.frame regl draw;
