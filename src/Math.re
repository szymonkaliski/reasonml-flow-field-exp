let pi = [%bs.raw "Math.PI"];

external sqrt : float => float = "Math.sqrt" [@@bs.val];
