## TDC-pancreas-artificial

UTN FRBA - Teoría de control: Código arduino de simulación de un páncreas artificial basado en la investigación del CONICET

### Demo

- **Demo en wokwi**: [Abrir la demo en wokwi](https://wokwi.com/projects/347496920546017875)
- **Nota**: para la demo se utiliza un potenciometro en lugar del sensor de agua para poder cambiar el valor de medición

#### Imagen de demo con hardware

![image](https://user-images.githubusercontent.com/4097554/200192254-f3120795-0d62-47e2-8c90-13e850537b6e.png)

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

- **Planilla**: [link de drive](https://docs.google.com/spreadsheets/d/1eZSCC03KBxAKdVK58ssGJT3xnhAzRS-QLdvUbbb3vlo/edit?usp=sharing)

t|dt|Oi (valorRef)|Sensor de agua|f|e (Oi - f)|e previo|e diff (e - e previo) /tiempo|proporcional (e < 0)|derivativo con tiempo|OR|Unidades de insulina|siguiente dt|Valor final esperado
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
0|0|180|0|50|130|null|null|FALSO|FALSO|FALSO|0|1000|50
1000|1000|180|512|175|5|null|null|FALSO|FALSO|FALSO|0|500|175
1500|500|180|512|175|5|5|0|FALSO|FALSO|FALSO|0|500|175
2000|500|180|450|160|20|5|30|FALSO|FALSO|FALSO|0|500|160
2500|500|180|390|145|35|20|30|FALSO|FALSO|FALSO|0|500|145
3000|500|180|450|160|20|35|-30|FALSO|FALSO|FALSO|0|500|160
3500|500|180|512|175|5|20|-30|FALSO|FALSO|FALSO|0|500|175
4000|500|180|600|197|-17|5|-44|VERDADERO|VERDADERO|VERDADERO|2|17000|97
21000|17000|180|1023|300|-120|-17|-6,058823529|VERDADERO|FALSO|VERDADERO|3|17000|150
38000|17000|180|700|221|-41|-120|4,647058824|VERDADERO|FALSO|VERDADERO|2|17000|121
55000|17000|180|570|189|-9|-41|1,882352941|VERDADERO|FALSO|VERDADERO|2|17000|89
72000|17000|180|300|123|57|-9|3,882352941|FALSO|FALSO|FALSO|0|1000|123
73000|1000|180|275|117|63|57|6|FALSO|FALSO|FALSO|0|1000|117
74000|1000|180|250|111|69|63|6|FALSO|FALSO|FALSO|0|1000|111
75000|1000|180|200|99|81|69|12|FALSO|FALSO|FALSO|0|1000|99
76000|1000|180|150|87|93|81|12|FALSO|FALSO|FALSO|0|1000|87
77000|1000|180|150|87|93|93|0|FALSO|FALSO|FALSO|0|1000|87
78000|1000|180|500|172|8|93|-85|FALSO|VERDADERO|VERDADERO|1|17000|122
95000|17000|180|300|123|57|8|2,882352941|FALSO|FALSO|FALSO|0|1000|123

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
