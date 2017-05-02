type dom;

type context;

external createElement : string => dom = "document.createElement" [@@bs.val];
external appendChild : dom => dom = "document.body.appendChild" [@@bs.val];
external getContext : dom => string => context = "getContext" [@@bs.send];
external fillRect : context => int => int => int => int => context = "fillRect" [@@bs.send];

let canvas = createElement "canvas";
let ctx = getContext canvas "2d";

appendChild canvas;

fillRect ctx 10 10 100 100;
