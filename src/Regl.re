type drawConfigT;
type drawComandT = unit => unit;
type reglT;

external make : Document.element => reglT = "regl" [@@bs.module];

/* HACK: I couldn't find better way of renaming a function */
let makeDrawCommand: reglT => drawConfigT => drawComandT = [%bs.raw
  "function(regl, drawCommand) { return regl(drawCommand); }"
];

/* HACK: otherwise function get's curried and is not run */
let drawCommand: drawComandT => unit = [%bs.raw
  "function(drawCommand) { return drawCommand(); }"
];

external makeDrawConfig :
  frag::string => vert::string => attributes::'a? => uniforms::'b? => count::int => drawConfigT =
  "" [@@bs.obj];

external frame : reglT => (unit => unit) => unit = "frame" [@@bs.send];
