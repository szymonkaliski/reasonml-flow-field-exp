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

let setCanvasSize _ => {
  let width = (Document.getWidth Document.window);
  let height = (Document.getHeight Document.window);

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

  Array.mapi (fun i _ => {
    callback i;
  }) emptyArray;
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

let particleCount = 2000;
let randomIdx = Random.int particleCount;
state.points = genItems particleCount (fun i => genPoint (i == randomIdx));

let drawCircle (ctx, x, y, r) => {
  Canvas.arc ctx x y r 0. (2. *. Math.pi) false;
};

let draw state => {
  let width = float_of_int state.window.width;
  let height = float_of_int state.window.height;

  Canvas.clearRect ctx 0. 0. width height;

  Array.iter (fun (point: pointT) => {
    let color = point.locked
      ? "rgba(120, 200, 110, 0.8)"
      : "rgba(20, 120, 200, 0.1)";

    Canvas.fillStyle ctx color;

    Canvas.beginPath ctx;

    let x = point.pos.x *. width;
    let y = point.pos.y *. height;

    drawCircle (ctx, x, y, point.size *. width);

    Canvas.fill ctx;
  }) state.points;
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let modPos = 1.0;
let modTime = 0.005;
let modVel = 0.99;
let modDir = 0.05;
let modSpeed = 0.02;

let dist a b => {
  let x = a.x -. b.x;
  let y = a.y -. b.y;

  Math.sqrt((x *. x) +. (y *. y));
};

type breakT = { mutable hit: bool };
let checkHit point pointIdx points => {
  let break: breakT = { hit: false };

  for idx in 0 to (Array.length points - 1) {
    let otherPoint = Array.get points idx;

    if (not break.hit && (pointIdx != idx) && otherPoint.locked) {
      let d = dist point.pos otherPoint.pos;

      if (d < point.size +. otherPoint.size) {
        break.hit = true;
      };
    }
  };

  break.hit;
};

let update events state => {
  for idx in 0 to (Array.length state.points - 1) {
    let point = Array.get state.points idx;

    if (not point.locked) {
      point.locked = checkHit point idx state.points;

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

  /* if (Random.float 1.0 < 0.001) { */
  /*   let newPoints = genItems 10 (fun _ => genPoint false); */
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
