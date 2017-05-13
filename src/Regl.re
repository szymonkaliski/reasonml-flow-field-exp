type drawConfigT;
type drawCommandT = unit  => unit;
type regl = drawConfigT => drawCommandT;

/* type clearT = [%bs.obj: */
/*   { */
/*     . */
/*     color: array int, */
/*     depth: int */
/*   } */
/* ] [@bs]; */

external makeRegl : Canvas.context => regl  = "regl" [@@bs.module] [@@bs.new] ;

external makeDrawConfig :
  frag::string => vert::string => attributes::'a? => uniforms::'b? => count::int => drawConfigT =
  "" [@@bs.obj];

/* external clear : regl => clearT => unit = "clear" [@@bs.send]; */
external frame : regl => (unit => unit) => unit = "frame" [@@bs.send];

