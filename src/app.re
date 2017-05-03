let module MouseEvent = {
  type event;

  external getClientX : event => int = "clientX" [@@bs.get];
  external getClientY : event => int = "clientY" [@@bs.get];
};

let module Document = {
  type element;

  let window: element = [%bs.raw "window"];

  external createElement : string => element = "document.createElement" [@@bs.val];
  external appendChild : element => element = "document.body.appendChild" [@@bs.val];
  external addEventListener : element => string => (MouseEvent.event => unit) => unit = "addEventListener" [@@bs.send];

  external getWidth : element => int = "innerWidth" [@@bs.get];
  external getHeight : element => int = "innerHeight" [@@bs.get];

  external setWidth : element => int => unit = "width" [@@bs.set];
  external setHeight : element => int => unit = "height" [@@bs.set];

  external requestAnimationFrame : (unit => unit) => unit = "requestAnimationFrame" [@@bs.val];
};

let module Canvas = {
  type context;

  external getContext : Document.element => string => context = "getContext" [@@bs.send];

  external fillRect : context => float => float => float => float => unit = "fillRect" [@@bs.send];
  external clearRect : context => float => float => float => float => unit  = "clearRect" [@@bs.send];
};

/* canvas/context setup */
let canvas = Document.createElement "canvas";
let ctx = Canvas.getContext canvas "2d";

Document.appendChild canvas;

let setCanvasSize _ => {
  Document.setWidth canvas (Document.getWidth Document.window);
  Document.setHeight canvas (Document.getHeight Document.window);
};

Document.addEventListener Document.window "resize" setCanvasSize;

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
  events.mouseX = MouseEvent.getClientX e;
  events.mouseY = MouseEvent.getClientY e;
});

Document.addEventListener canvas "mousedown" (fun _ => {
  events.mouseDown = true;
});

Document.addEventListener canvas "mouseup" (fun _ => {
  events.mouseDown = false;
});

/* app code */

type pointT = {
  mutable x: float,
  mutable y: float
};

type stateT = {
  mutable points: list pointT
};

let state : stateT = {
  points: []
};

let draw state => {
  Canvas.clearRect ctx 0. 0. 300. 150.;

  List.map (fun point => {
    Canvas.fillRect ctx point.x point.y 10. 10.;
  }) state.points;
};

let update events state => {
  if (events.mouseDown) {
    let point: pointT = {
      x: float_of_int events.mouseX,
      y: float_of_int events.mouseY
    };

    state.points = state.points @ [point];
  }
};

let rec loop () => {
  update events state;
  draw state;

  Document.requestAnimationFrame loop;
};

Document.requestAnimationFrame loop;
