/* 2017-05-07: Flow Field with Simplex noise */

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
  vel: vecT
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

state.points = genItems 5000 (fun _ => {
  {
    pos: {
      x: Random.float 1.,
      y: Random.float 1.
    },
    vel: {
      x: 0.,
      y: 0.
    }
  };
});

let drawCircle (ctx, x, y, r) => {
  Canvas.arc ctx x y r 0. (2. *. Math.pi) false;
};

let draw state => {
  let width = float_of_int state.window.width;
  let height = float_of_int state.window.height;

  Canvas.fillStyle ctx "rgba(255, 255, 255, 0.001)";
  Canvas.fillRect ctx 0. 0. width height;

  let r = 1.;

  Canvas.fillStyle ctx "rgba(20, 120, 200, 0.20)";

  Array.iter (fun point => {
    Canvas.beginPath ctx;

    let x = point.pos.x *. width;
    let y = point.pos.y *. height;

    drawCircle (ctx, x, y, r);

    Canvas.fill ctx;
  }) state.points;
};

let angleToVec angle :vecT => {
  x: (sin (angle *. 2. *. Math.pi)),
  y: (cos (angle *. 2. *. Math.pi))
};

let modPos = 3.0;
let modIdx = 0.2;
let modVel = 0.2;
let modDir = 0.2;

let update _ state => {
  state.points = Array.mapi (fun i point => {
    let nx = point.pos.x *. modPos;
    let ny = point.pos.y *. modPos;
    let nz = ((float_of_int i) /. (float_of_int (Array.length state.points))) *. modIdx;

    let noise = noise3D simplex nx ny nz;
    let direction = angleToVec noise;

    point.vel.x = point.vel.x +. direction.x *. modDir;
    point.vel.y = point.vel.y +. direction.y *. modDir;

    point.vel.x = point.vel.x *. modVel;
    point.vel.y = point.vel.y *. modVel;

    point.pos.x = point.pos.x +. point.vel.x;
    point.pos.y = point.pos.y +. point.vel.y;

    point;
  }) state.points;
};

let rec loop () => {
  update events state;
  draw state;

  Document.requestAnimationFrame loop;
};

Document.requestAnimationFrame loop;
