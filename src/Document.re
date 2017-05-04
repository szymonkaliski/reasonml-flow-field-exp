type element;

let window: element = [%bs.raw "window"];

external createElement : string => element = "document.createElement" [@@bs.val];
external appendChild : element => element = "document.body.appendChild" [@@bs.val];
external addEventListener : element => string => (Events.event => unit) => unit = "addEventListener" [@@bs.send];

external getWidth : element => int = "innerWidth" [@@bs.get];
external getHeight : element => int = "innerHeight" [@@bs.get];

external setWidth : element => int => unit = "width" [@@bs.set];
external setHeight : element => int => unit = "height" [@@bs.set];

external requestAnimationFrame : (unit => unit) => unit = "requestAnimationFrame" [@@bs.val];
