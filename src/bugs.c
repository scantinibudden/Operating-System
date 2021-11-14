BUGS ACTUALIZADOS

-si un equipo tiene un bridge y el otro no, despues del primer bridge, cada turno del equipo con el bridge
son dos movimientos y cada turno del equipo sin bridge es 1 movimiento, pasa lo mismo si en vez de bridge es explode
-a veces cuando el b tiene un explode, el a se imprime en pantalla en donde esta, no queda registrado en el juego, 
solo en la pantalla --> no encontramos donde puede estar el problema, suele ser en la misma posicion siempre pero 
esa es la unica constante, a veces pasa y otras veces no

//SOLUCIONADOS
-el reloj de un b desalojado no se borra
-el sexto lemming se crea pero no se mueve, SOLO SI HAY UN LEMMING EN EL EQUIPO
-si hay un bridge intercalado entre movimientos y el bridge no se puede hacer, el lemming se queda trabado,
y no hace ningun otro movimiento. --> no nos dimos cuenta que pueda o no hacerse el bridge, el lemming hay que desalojarlo