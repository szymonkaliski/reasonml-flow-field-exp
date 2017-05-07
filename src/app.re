type simplex;
external newSimplex : unit => simplex = "simplex-noise" [@@bs.module] [@@bs.new];
external noise2D : simplex => float => float => float = "noise2D" [@@bs.send];
external noise3D : simplex => float => float => float => float = "noise3D" [@@bs.send];

let simplex = newSimplex ();

let module Math = {
  let pi = [%bs.raw "Math.PI"];
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
  locked: bool,
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
  minSize: 2.,
  maxSize: 6.
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
  mutable mouseDown: bool
};

let events : eventsT = {
  mouseX: 0,
  mouseY: 0,
  mouseDown: false
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

  Array.map (fun i => {
    callback i;
  }) emptyArray;
};

state.points = genItems 100 (fun _ => {
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
    locked: false
  };
});

let drawCircle (ctx, x, y, r) => {
  Canvas.arc ctx x y r 0. (2. *. Math.pi) false;
};

let draw state => {
  let width = float_of_int state.window.width;
  let height = float_of_int state.window.height;

  Canvas.clearRect ctx 0. 0. width height;

  Canvas.fillStyle ctx "rgba(20, 120, 200, 200.0)";

  Array.iter (fun (point: pointT) => {
    Canvas.beginPath ctx;

    let x = point.pos.x *. width;
    let y = point.pos.y *. height;

    drawCircle (ctx, x, y, point.size);

    Canvas.fill ctx;
  }) state.points;
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let modPos = 6.0;
let modIdx = 0.02;
let modVel = 0.95;
let modDir = 0.02;
let modSpeed = 0.01;

let update _ state => {
  for idx in 0 to (Array.length state.points - 1) {
    let point = Array.get state.points idx;

    /* if point.locked { */
    /*   print_endline "locked"; */
    /* }; */

    let nx = point.pos.x *. modPos;
    let ny = point.pos.y *. modPos;
    let nz = ((float_of_int idx) /. (float_of_int (Array.length state.points))) *. modIdx;

    let noise = noise3D simplex nx ny nz;
    let direction = angleToVec noise;

    point.vel.x = point.vel.x +. direction.x *. modDir;
    point.vel.y = point.vel.y +. direction.y *. modDir;

    point.vel.x = point.vel.x *. modVel;
    point.vel.y = point.vel.y *. modVel;

    point.pos.x = point.pos.x +. point.vel.x *. modSpeed *. (1. -. point.size /. consts.maxSize);
    point.pos.y = point.pos.y +. point.vel.y *. modSpeed *. (1. -. point.size /. consts.maxSize);
  };
};

let rec loop () => {
  update events state;
  draw state;

  Document.requestAnimationFrame loop;
};

Document.requestAnimationFrame loop;
