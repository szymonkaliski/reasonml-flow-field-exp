type simplex;
external newSimplex : unit => simplex = "simplex-noise" [@@bs.module] [@@bs.new];
external noise2D : simplex => float => float => float = "noise2D" [@@bs.send];

let simplex = newSimplex ();

/* main app state */

type windowT = {
  mutable width: int,
  mutable height: int
};

type pointT = {
  mutable x: float,
  mutable y: float
};

type stateT = {
  mutable points: list pointT,
  window: windowT,
};

let state : stateT = {
  points: [],
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

/* TODO: for-loop, this blows up stack */
let genItems num callback => {
  let rec buildItems num callback items => {
    if (num > 0) {
      let newItem = callback (num);

      buildItems (num - 1) callback (items @ [newItem]);
    } else {
      items;
    }
  };

  buildItems num callback [];
};

state.points = genItems 2000 (fun _ => {
  {
    x: Random.float 1.,
    y: Random.float 1.
  }
});

let drawCircle (ctx, x, y, r) => {
  let pi = [%bs.raw "Math.PI"];

  Canvas.arc ctx x y r 0. (2. *. pi) false;
};

let draw state => {
  let width = float_of_int state.window.width;
  let height = float_of_int state.window.height;

  Canvas.clearRect ctx 0. 0. width height;

  let r = 2.;

  Canvas.strokeStyle ctx "rgba(200, 120, 100)";

  List.iter (fun point => {
    Canvas.beginPath ctx;

    let x = point.x *. width;
    let y = point.y *. height;

    drawCircle (ctx, x, y, r);

    Canvas.stroke ctx;
  }) state.points;
};

let modx = 1.;
let mody = 1.;

let update events state => {
  state.points = List.mapi (fun i point => {
    let noise = noise2D simplex (point.x *. modx) (point.y *. mody *. ((float_of_int i) /. 1000.));

    let point = {
      x: point.x +. ((sin noise) *. 0.01) *. 0.05,
      y: point.y +. ((cos noise) *. 0.01) *. 0.05
    };

    point;
  }) state.points;
};

let rec loop () => {
  update events state;
  draw state;

  Document.requestAnimationFrame loop;
};

Document.requestAnimationFrame loop;
