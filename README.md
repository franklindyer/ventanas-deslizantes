# ventanas-deslizantes

![Fig1](/img/ventanas-deslizantes.png)

Una simulación en C del algoritmo de ventanas deslizantes que se usa en el protocolo TCP. Se describe el algoritmo con 4 parámetros:

- el tamaño de la ventana
- el número total de segmentos
- el tiempo de transmisión de un segmento
- la duración antes de un timeout

Para ejecutar la simulación se usa o el ejecutable `vd_resumen.o` o el ejecutable `vd_verbose.o` con los cuatro parámatros pasados como argumentos en la línea de ordenes. Además acepta (por entrada estándar) una lista de floats que describen los retrasos de propagación de los paquetes. Por ejemplo para poner en marcha una simulación de la política "stop and wait" (tamaño de ventana igual a 1) con 10 segmentos por total, un tiempo de transmisión de 0.2 segundos y un timeout de 10 segundos usando una lista de retardos de un fichero de texto `retardos_prueba.txt` se puede ejecutar
```
./vd_resumen.o  1 10 0.2 10 < retardos_prueba.txt
```
Un tiempo de transmisión de `-1` se interpreta como la pérdida de un segmento. Para generar los retardos usando un variable aleatorio con la distribución exponencial con promedio igual a 0.5 segundos y probabilidad 0.1 de pérdida de segmento se puede usar un pipe con el script `propags.py`:
```
python3 propags.py 0.1 0.5 | ./vd_resumen.o 1 10 0.2 10
```
La versión no verboso producirá salida así:
```
---RESUMEN---
Duración total: 108.3859
Tiempo ocioso del emisor: 105.3859
Segmentos transmitidos: 16
Número de segmentos transmitidos que resultaron perdidos: 4
ACKs transmitidos: 12
Número de ACKs transmitidos que resultaron perdidos: 2
```
mientras que la versión verbosa produce una lista de los eventos que suceden paso por paso en orden cronológico:
```
0 : Transimision del primer segmento.
0.2000 : fin de transmision de segmento 0.
2.4260 : llega el segmento 0.
3.0827 : llega un ACK que pide el siguiente segmento 1.
3.0827 : principio de transmision de segmento 1.
3.9062 : llega el segmento 1.
...
```
