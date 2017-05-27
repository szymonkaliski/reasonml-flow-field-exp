/* main app state */

type windowT = {
  mutable width: int,
  mutable height: int
};

type vecT = {
  mutable x: float,
  mutable y: float
};

type pointT = {
  pos: vecT,
  vel: vecT,
  mutable size: float,
  mutable life: float,
  mutable lockedSize: float,
  mutable locked: bool,
};

type stateT = {
  mutable points: array pointT,
  window: windowT,
};

let state : stateT = {
  points: [| |],
  window: {
    width: 0,
    height: 0
  }
};

/* regl setup */

let canvas = Document.createElement "canvas";

Document.appendChild canvas;

let setCanvasSize _ => {
  let width = Document.getWidth Document.window;
  let height = Document.getHeight Document.window;

  Document.setWidth canvas width;
  Document.setHeight canvas height;

  state.window.width = width;
  state.window.height = height;
};

Document.addEventListener Document.window "resize" setCanvasSize;
Document.addEventListener Document.window "DOMContentLoaded" setCanvasSize;

Regl.create canvas;

/* app */

let simplex = Simplex.newSimplex ();

let numPoints = 250;

let modPos = 20.;
let modTime = 0.9;
let modVel = 0.25;
let modDir = 0.2;
let modSpeed = 1.00;
let modDie = 0.03;
let modDist = 200.;
let stickChance = 0.09;

let warmupCount = 120;

let drawMetaballsComand = Regl.makeDrawCommand (Obj.magic [%bs.obj {
  vert: "
    attribute vec2 position;

    void main() {
      gl_Position = vec4(position, 0., 1.);
    }
  ",
  frag: "
    precision mediump float;

    uniform float width, height;
    uniform vec3 metaballs[" ^ (string_of_int numPoints) ^ "];

    void main() {
      float x = gl_FragCoord.x;
      float y = gl_FragCoord.y;

      vec3 color = vec3(44., 39., 79.) / vec3(255.);
      vec3 background = vec3(227., 224., 218.) / vec3(255.);

      float v = 0.0;

      for (int i = 0; i < " ^ (string_of_int numPoints) ^ "; i++) {
        vec3 metaball = metaballs[i];
        float r = metaball.z;

        if (r > 0.0) {
          float dx = (metaball.x * width)  - x;
          float dy = (metaball.y * height) - y;

          v += ((r * r) / (dx * dx + dy * dy)) * 9.;
        }
      }

      float min = 0.8;
      float max = 1.2;

      if (v > min && v < max) {
        float scaled = (v - min) / (max - min);
        float lerpValue = 1. - sin(scaled * 3.1415);

        vec3 outColor = mix(color, background, lerpValue);

        gl_FragColor = vec4(outColor, 1.0);
      } else {
        gl_FragColor = vec4(background, 1.0);
      }
    }
  ",
  uniforms: Js.Obj.assign
    (Obj.magic (Regl.unrollUniformForProp "metaballs" numPoints))
    [%bs.obj {
      width: Regl.prop "width",
      height: Regl.prop "height"
    }],
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

let genPoint locked => {
  {
    pos: {
      x: Random.float 1.,
      y: Random.float 1.
    },
    vel: {
      x: Random.float 1. -. 0.5,
      y: Random.float 1. -. 0.5
    },
    size: Random.float 2. +. 1.,
    life: 10.,
    lockedSize: 0.,
    locked: locked
  };
};

state.points = genItems numPoints (fun _ => genPoint ((Random.float 1.0) > 0.99));

let metaballsFromPoints points => {
  Array.map (fun point => {
    [|
      point.pos.x,
      point.pos.y,
      point.lockedSize *. min 1.0 point.life,
    |]
  }) points;
};

let dist a b => {
  let x = a.x -. b.x;
  let y = a.y -. b.y;

  Math.sqrt((x *. x) +. (y *. y));
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let checkHit point lockedPoints => {
  let idx = Js.Array.findIndex (fun lockedPoint => {
    let d = dist point.pos lockedPoint.pos;

    (d < (point.size +. lockedPoint.size) /. modDist);
  }) lockedPoints;

  idx >= 0;
};

let update t => {
  let lockedPoints = Js.Array.filter (fun point => point.locked) state.points;

  if ((Array.length lockedPoints) == 0) {
    let randomIdx = Random.int (Array.length state.points);
    let randomPoint = Array.unsafe_get state.points randomIdx;
    randomPoint.locked = true;
  };

  for idx in 0 to (Array.length state.points - 1) {
    let point = Array.unsafe_get state.points idx;

    if (point.locked) {
      point.lockedSize = min (point.lockedSize +. 0.01) point.size;

      if (point.lockedSize >= 0.99) {
        point.life = point.life -. modDie;

        if (point.life < 0.0) {
          Array.unsafe_set state.points idx (genPoint false);
        }
      }
    }
    else {
      let nx = point.pos.x *. modPos;
      let ny = point.pos.y *. modPos;
      let nz = t *. modTime;

      let noise = Simplex.noise3D simplex nx ny nz;
      let direction = angleToVec noise;

      point.vel.x = point.vel.x +. direction.x *. modDir;
      point.vel.y = point.vel.y +. direction.y *. modDir;

      point.vel.x = point.vel.x *. modVel;
      point.vel.y = point.vel.y *. modVel;

      point.pos.x = point.pos.x +. point.vel.x *. modSpeed;
      point.pos.y = point.pos.y +. point.vel.y *. modSpeed;

      if (point.pos.x > 1.0) { point.pos.x = 0.0; };
      if (point.pos.y > 1.0) { point.pos.y = 0.0; };
      if (point.pos.x < 0.0) { point.pos.x = 1.0; };
      if (point.pos.y < 0.0) { point.pos.y = 1.0; };

      if (checkHit point lockedPoints) {
        if (Random.float 1.0 < stickChance) {
          point.locked = true;
        }
      };
    };
  };
};

let draw t => {
  update t;

  Regl.clear
    color::[| 0, 0, 0, 1 |]
    depth::1;

  let metaballsUniforms = Obj.magic [%bs.obj {
    metaballs: metaballsFromPoints state.points,
    width: state.window.width,
    height: state.window.height,
  }];

  Regl.draw
    command::drawMetaballsComand
    uniforms::metaballsUniforms;
};

/* warmup */
for idx in 0 to warmupCount {
  let t = (float_of_int idx) /. (float_of_int warmupCount);

  update t;
};

Regl.frame draw;
