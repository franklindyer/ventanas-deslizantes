# ventanas-deslizantes

Una simulación en C del algoritmo de ventanas deslizantes que se usa en el protocolo TCP. Se describe el algoritmo con 4 parámetros:
- el tamaño de la ventana
- el número total de segmentos
- el tiempo de transmisión de un segmento
- la duración antes de un timeout
Para ejecutar la simulación se usa el ejecutable `a.out` con los cuatro parámatros pasados como argumentos en la línea de ordenes. Además acepta (por input estándar) una lista de floats que describen los retrasos de propagación de los paquetes.
