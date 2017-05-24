type simplex;

external newSimplex : unit => simplex = "simplex-noise" [@@bs.module] [@@bs.new];
external noise2D : simplex => float => float => float = "noise2D" [@@bs.send];
external noise3D : simplex => float => float => float => float = "noise3D" [@@bs.send];

