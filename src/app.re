type dom;
type context;
type requestId;

external createElement : string => dom = "document.createElement" [@@bs.val];
external appendChild : dom => dom = "document.body.appendChild" [@@bs.val];
external getContext : dom => string => context = "getContext" [@@bs.send];
external fillRect : context => int => int => int => int => context = "fillRect" [@@bs.send];
external clearRect : context => int => int => int => int => context = "clearRect" [@@bs.send];

external requestAnimationFrame : (unit => unit) => unit = "requestAnimationFrame" [@@bs.val];

let canvas = createElement "canvas";
let ctx = getContext canvas "2d";

appendChild canvas;

type stateT = {mutable x: int};
let state : stateT = {x: 0};

let draw state => {
  clearRect ctx 0 0 300 150;
  fillRect ctx state.x 10 10 10;
};

let update state => {
  state.x = state.x + 1;
};

let rec loop () => {
  update state;
  draw state;

  requestAnimationFrame loop;
};

loop ();
