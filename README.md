## TDC-pancreas-artificial

UTN FRBA - Teoría de control: Código arduino de simulación de un páncreas artificial basado en la investigación del CONICET

### Se hace un control PD (proporcional-derivativo)

#### Parámetros

- Valor de referencia: `180 mg/dl`
- Glucosa: medición del sensor de agua mapeado de `0 a 1023` (**sensor agua**) con `50 a 300` (**f**)
- Punto suma: retroalimentación negativa `e = ValorRef - f`
- Proporcional: si `e < 0`
- Derivativo: si `de/dt < -30`

#### Tiempos de medición

- Si los valores de glucemia están entre `120` y `180` (`60` menos del valor de referencia) se toman mediciones cada `500` milisegundos
- Si los valores de glucemia están por debajo de `120` (`60` menos del valor de referencia) se toman mediciones cada `1000` milisegundos
- Si los valores de glucemia supera los `180` (valor de referencia) se esperan `2000` milisegundos para inyección de insulina y `15000` de espera de metabolización

#### Calculo derivativo

- Se calcula en tiempo entre mediciones (`dt = tiempoMedicionActual - tiempoMedcionAnterior`)
- Calculo de `dedt` -> `(e-ePrevio) / (dt / 1000)`
- Si el valor de derivativo (`dedt`) es menor a `-30`

### Tabla de valores

|Oi (valorRef)|Sensor de agua|t|dt|f|e (Oi - f)|e previo|de/dt|proporcional (e < 0)|derivativo (de/dt < -30)|OR|siguiente dt|
|---|---|---|---|---|---|---|---|---|---|---|---|
|180|0|0|0|50|130|null|null|FALSO|FALSO|FALSO|1000|
|180|512|1000|1000|175|5|null|null|FALSO|FALSO|FALSO|500|
|180|512|1500|500|175|5|5|0|FALSO|FALSO|FALSO|500|
|180|450|2000|500|160|20|5|30|FALSO|FALSO|FALSO|500|
|180|390|2500|500|145|35|20|30|FALSO|FALSO|FALSO|500|
|180|450|3000|500|160|20|35|-30|FALSO|FALSO|FALSO|500|
|180|512|3500|500|175|5|20|-30|FALSO|FALSO|FALSO|500|
|180|600|4000|500|197|-17|5|-44|VERDADERO|VERDADERO|VERDADERO|17000|
|180|800|21000|17000|246|-66|-17|-2,882352941|VERDADERO|FALSO|VERDADERO|17000|
|180|570|38000|17000|189|-9|-66|3,352941176|VERDADERO|FALSO|VERDADERO|17000|
|180|300|55000|17000|123|57|-9|3,882352941|FALSO|FALSO|FALSO|1000|
|180|275|56000|1000|117|63|57|6|FALSO|FALSO|FALSO|1000|
|180|250|57000|1000|111|69|63|6|FALSO|FALSO|FALSO|1000|
|180|200|58000|1000|99|81|69|12|FALSO|FALSO|FALSO|1000|
|180|150|59000|1000|87|93|81|12|FALSO|FALSO|FALSO|1000|
|180|150|60000|1000|87|93|93|0|FALSO|FALSO|FALSO|1000|
|180|500|61000|1000|172|8|93|-85|FALSO|VERDADERO|VERDADERO|17000|
|180|300|78000|17000|123|57|8|2,882352941|FALSO|FALSO|FALSO|1000|

### Gráfico de tabla de valores

![Screenshot from 2022-11-06 15-58-04](https://user-images.githubusercontent.com/4097554/200191149-89ff518a-fbb6-4e1c-a145-d4da0b08d509.png)

### Ver en consola de linux

```bash
stty -F /dev/ttyACM0 raw 9600
cat /dev/ttyACM0
```

### Imagenes

#### Valor normal (bajo) -> dt = 1000

![Screenshot from 2022-11-06 15-55-34](https://user-images.githubusercontent.com/4097554/200191160-d2123fba-e53c-412b-ad1a-81093da9eab0.png)

#### Valor alto (pasado de 180) -> activación por proporcional -> inyección de insulina

![Screenshot from 2022-11-06 15-55-38](https://user-images.githubusercontent.com/4097554/200191158-bae306c7-5763-4be3-a1bb-1f638df9f06f.png)

#### Valor alto (pasado de 180) -> espera post inyección por proporcional

![Screenshot from 2022-11-06 15-55-40](https://user-images.githubusercontent.com/4097554/200191157-da3cd858-0e74-4e60-9551-a1ac92285ec5.png)

#### Valor normal (medio-alto) -> dt = 500

![Screenshot from 2022-11-06 15-55-56](https://user-images.githubusercontent.com/4097554/200191156-abd90f8e-a37c-4976-924d-98195c4b389f.png)

#### Valor normal (bajo) -> dt = 1000

![Screenshot from 2022-11-06 15-56-20](https://user-images.githubusercontent.com/4097554/200191155-6a7057ac-b488-4d76-bc51-4df5318a88fe.png)

#### Valor normal (alto -> warning) con activación por derivativo

![Screenshot from 2022-11-06 15-56-24](https://user-images.githubusercontent.com/4097554/200191154-32a9eccb-01f2-4613-a70d-f96f3687e5e9.png)

#### Valor normal (alto -> warning) con post activación derivativo (después de espera)

![Screenshot from 2022-11-06 15-56-46](https://user-images.githubusercontent.com/4097554/200191152-795948b9-6021-4d36-a51f-1967bc9eaa25.png)
