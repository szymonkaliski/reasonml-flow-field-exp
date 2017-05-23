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

let numMetaballs = 500;

let drawMetaballsComand = Regl.makeDrawCommand (Obj.magic [%bs.obj {
  vert: "
    attribute vec2 position;

    void main() {
      gl_Position = vec4(position, 0., 1.);
    }
  ",
  frag: "
    precision mediump float;

    uniform vec3 metaballs[" ^ (string_of_int numMetaballs) ^ "];

    void main() {
      float x = gl_FragCoord.x;
      float y = gl_FragCoord.y;

      float v = 0.0;

      for (int i = 0; i < " ^ (string_of_int numMetaballs) ^ "; i++) {
        vec3 metaball = metaballs[i];

        float dx = (metaball.x * 700.) - x;
        float dy = (metaball.y * 800.) - y;
        float r = metaball.z;

        v += r * r / (dx * dx + dy * dy) * 1.2;
      }

      if (v > 1.0) {
        gl_FragColor = vec4(vec3(v * 0.8), 1.0);
      } else {
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
      }
    }
  ",
  uniforms: Obj.magic (Regl.unrollUniformForProp "metaballs" numMetaballs),
  count: 6,
  attributes: {
    position: [|
      [| -1., -1. |],
      [| 1., 1. |],
      [| -1., 1. |],
      [| 1., 1. |],
      [| 1., -1. |],
      [| -1., -1. |]
    |]
    }
}]);

let genItems num callback => {
  let emptyArray = Array.make num 0;

  Array.mapi (fun i _ => callback i) emptyArray;
};

let genMetaball _ => {
  [|
    Random.float 1.,
    Random.float 1.,
    Random.float 10.
  |]
};

let metaballs = genItems numMetaballs genMetaball;

let draw t => {
  Regl.clear
    color::[| 0, 0, 0, 1 |]
    depth::1;

  let metaballsUniforms = Obj.magic [%bs.obj {
    metaballs: metaballs
  }];

  Regl.draw
    command::drawMetaballsComand
    uniforms::metaballsUniforms;
};

Regl.frame draw;
