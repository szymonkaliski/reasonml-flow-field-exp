type simplex;
external newSimplex : unit => simplex = "simplex-noise" [@@bs.module] [@@bs.new];
external noise2D : simplex => float => float => float = "noise2D" [@@bs.send];
external noise3D : simplex => float => float => float => float = "noise3D" [@@bs.send];

let simplex = newSimplex ();

let module Math = {
  let pi = [%bs.raw "Math.PI"];

  external sqrt : float => float = "Math.sqrt" [@@bs.val];
};

let module Date = {
  external now : unit => float = "Date.now" [@@bs.val];
};

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
  size: float,
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

type constsT = {
  minSize: float,
  maxSize: float
};

let consts: constsT = {
  minSize: 0.002,
  maxSize: 0.006
};

/* canvas/context setup */
let canvas = Document.createElement "canvas";
let ctx = Canvas.getContext canvas "2d";

Document.appendChild canvas;

let maxWidth = 400;
let maxHeight = 400;

let setCanvasSize _ => {
  let width = min (Document.getWidth Document.window) maxWidth;
  let height = min (Document.getHeight Document.window) maxHeight;

  Document.setWidth canvas width;
  Document.setHeight canvas height;

  state.window.width = width;
  state.window.height = height;
};

Document.addEventListener Document.window "resize" setCanvasSize;
Document.addEventListener Document.window "DOMContentLoaded" setCanvasSize;

/* events */
type eventsT = {
  mutable mouseX: int,
  mutable mouseY: int,
  mutable mouseDown: bool,
  mutable time: float
};

let events : eventsT = {
  mouseX: 0,
  mouseY: 0,
  mouseDown: false,
  time: 0.
};

Document.addEventListener canvas "mousemove" (fun e => {
  events.mouseX = Events.getClientX e;
  events.mouseY = Events.getClientY e;
});

Document.addEventListener canvas "mousedown" (fun _ => {
  events.mouseDown = true;
});

Document.addEventListener canvas "mouseup" (fun _ => {
  events.mouseDown = false;
});

/* app code */

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
    size: Random.float (consts.maxSize -. consts.minSize) +. consts.minSize,
    locked: locked
  };
};

let particleCount = 10;
let randomIdx = Random.int particleCount;
state.points = genItems particleCount (fun i => genPoint (i == randomIdx));

let drawCircle (ctx, x, y, r) => {
  Canvas.arc ctx x y r 0. (2. *. Math.pi) false;
};

let dist a b => {
  let x = a.x -. b.x;
  let y = a.y -. b.y;

  Math.sqrt((x *. x) +. (y *. y));
};

type pixelsDataT = {
  mutable idx: int,
  mutable pixels: array int,
  mutable tmpColor: int
};

let data: pixelsDataT = {
  idx: 0,
  pixels: [| |],
  tmpColor: 0
};

let draw state => {
  /* Canvas.clearRect ctx 0. 0. width height; */

  let pixelsCount = state.window.width * state.window.height;

  if (Array.length data.pixels != pixelsCount * 4) {
    data.pixels = Array.make (pixelsCount * 4) 0;
  };

  data.idx = 0;

  while (data.idx < pixelsCount * 4) {
    data.tmpColor = 0;

    for idx in 0 to (Array.length state.points - 1) {
      let point = Array.unsafe_get state.points idx;

      let y = float_of_int (data.idx / (state.window.width * 1));
      let x = float_of_int (data.idx mod (state.window.width * 4));

      let px = point.pos.x *. float_of_int state.window.width;
      let py = point.pos.y *. float_of_int state.window.width;

      let d = dist { x: x, y: y } { x: px, y: py };
      let color = point.size /. d;

      data.tmpColor = data.tmpColor + int_of_float (color *. 600000.);
    };

    Array.unsafe_set data.pixels (data.idx + 0) data.tmpColor;
    Array.unsafe_set data.pixels (data.idx + 1) data.tmpColor;
    Array.unsafe_set data.pixels (data.idx + 2) data.tmpColor;
    Array.unsafe_set data.pixels (data.idx + 3) 255;

    data.idx = data.idx + 4;
  };

  let typedData = TypedArray.makeUint8ClampedArray data.pixels;

  let imageData = Canvas.newImageData typedData state.window.width state.window.height;

  Canvas.putImageData ctx imageData 0 0;



  /* Array.iter (fun (point: pointT) => { */
  /*   if (point.locked) { */
  /*     let color = point.locked */
  /*       ? "rgba(120, 200, 110, 0.8)" */
  /*       : "rgba(20, 120, 200, 0.1)"; */

  /*     Canvas.fillStyle ctx color; */

  /*     Canvas.beginPath ctx; */

  /*     let x = point.pos.x *. width; */
  /*     let y = point.pos.y *. height; */

  /*     drawCircle (ctx, x, y, point.size *. size); */

  /*     Canvas.fill ctx; */
  /*   }; */
  /* }) state.points; */
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let modPos = 1.0;
let modTime = 0.005;
let modVel = 0.99;
let modDir = 0.05;
let modSpeed = 0.2;

let checkHit point lockedPoints => {
  let idx = Js.Array.findIndex (fun lockedPoint => {
    let d = dist point.pos lockedPoint.pos;

    d < point.size +. lockedPoint.size;
  }) lockedPoints;

  idx >= 0;
};

let update events state => {
  /* let lockedPoints = Js.Array.filter (fun point => point.locked) state.points; */

  for idx in 0 to (Array.length state.points - 1) {
    let point = Array.unsafe_get state.points idx;

    if (not point.locked) {
      /* if (checkHit point lockedPoints) { */
      /*   point.locked = true; */
      /* }; */

      let nx = point.pos.x *. modPos;
      let ny = point.pos.y *. modPos;
      let nz = events.time *. modTime;

      let noise = noise3D simplex nx ny nz;
      let direction = angleToVec noise;

      point.vel.x = point.vel.x +. direction.x *. modDir;
      point.vel.y = point.vel.y +. direction.y *. modDir;

      point.vel.x = point.vel.x *. modVel;
      point.vel.y = point.vel.y *. modVel;

      point.pos.x = point.pos.x +. point.vel.x *. modSpeed *. (1. -. point.size /. consts.maxSize);
      point.pos.y = point.pos.y +. point.vel.y *. modSpeed *. (1. -. point.size /. consts.maxSize);

      if (point.pos.x > 1.0) { point.pos.x = 0.0; };
      if (point.pos.y > 1.0) { point.pos.y = 0.0; };
      if (point.pos.x < 0.0) { point.pos.x = 1.0; };
      if (point.pos.y < 0.0) { point.pos.y = 1.0; };
    };
  };

  /* if (Random.float 1.0 < 0.01) { */
  /*   let newPoints = genItems 100 (fun _ => genPoint false); */
  /*   state.points = Array.append state.points newPoints; */
  /* }; */
};

let start = Date.now ();

let rec loop () => {
  let now = Date.now ();
  events.time = now -. start;

  update events state;
  draw state;

  Document.requestAnimationFrame loop;
};

Document.requestAnimationFrame loop;
