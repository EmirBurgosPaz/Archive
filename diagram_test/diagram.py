import os
os.environ["PATH"] += os.pathsep + "C:/Program Files (x86)/Graphviz/bin"

from diagrams import Diagram
from diagrams.c4 import Person, Container, Database, System, SystemBoundary, Relationship, Code

#Diagrama avanzado del sistema


graph_attr = {
    "splines": "spline",
}


with Diagram("Interfaz Arduino-Nextion", direction="TB", graph_attr=graph_attr):
    customer = Person(name="Usuario", description="Usuario del sistema")
    #
    Controlador = System(name="Arduino", description="Recive y manda datos de la interfaz", external=False)
    Sistema = System(name="Sistema fisico", description="Sistema que sera controlado", external=True)
    with SystemBoundary("Interfaz"):
        index = Container(
            name="Indice principal",
            technology="Nextion",
            description="Indice para escoger variables"
       )

        automatico = Container(
            name="Pagina automatico",
            technology="Nextion",
            description="Solo puedes modificar LEDs"
       )

        manual = Container(
            name="Pagina manual",
            technology="Nextion",
            description="Se puede modificar LEDs y las horas de la alarma"
       )

        configuracion = Container(
            name="Pagina configuracion",
            technology="Nextion",
            description="Te permite modifcar todo"
       )

        ModificadorLEDs = Container(
            name="Modificador LEDs",
            technology="Nextion",
            description="Te permite escoger los valores RGB de los LEDs"
       )
        AlarmSelecter = Container(
            name="Selector alarmas",
            technology="Nextion",
            description="Te permite escoger que alarma quieres modificar"
       )
        ModificadorHora = Container(
            name="Modificador hora",
            technology="Nextion",
            description="Te permite modificar la hora"
       )
        MotorSelecter = Container(
            name="Selector motor",
            technology="Nextion",
            description="Te permite escoger que motores quieres modificar"
       )
        MotoresSimples = Container(
            name="Motor 1-5",
            technology="Nextion",
            description="Te permite apagar/prender los motores simples"
       )
        Motor6 = Container(
            name="Modificador motor 6",
            technology="Nextion",
            description="Te permite modificar el motor 6"
       )


    customer >> Relationship("Modifica las opciones") >> index
    index >> Relationship("Escoges la opcion deseada") >> [automatico, manual, configuracion]
    automatico >> Relationship("Modificas solo los LEDs") >> ModificadorLEDs
    manual >> Relationship("Modifica horas y LEDs") >> [ModificadorLEDs, AlarmSelecter]
    AlarmSelecter >> Relationship("Escoges la alarma a modificar") >> ModificadorHora
    configuracion >> Relationship("Modifica todo") >> [ModificadorLEDs, AlarmSelecter, MotorSelecter]
    MotorSelecter >> Relationship("Escoges los motores a modificar") >> [MotoresSimples, Motor6]
    [Motor6,MotoresSimples,ModificadorHora,ModificadorLEDs] >> Relationship("Recive informacion y devuelve informacion") >> Controlador
    Controlador >> Relationship("Modifica los elementos fisicos") >> Sistema

with Diagram("Interfaz Arduino-Nextion simple", direction="TB", graph_attr=graph_attr):
    customer = Person(name="Usuario", description="Usuario del sistema")
    Controlador = System(name="Arduino", description="Recive y manda datos de la interfaz", external=False)
    Sistema = System(name="Sistema fisico", description="Sistema que sera controlado", external=True)
    interfazSimple = System(name="Interfaz", description="Indice para escoger variables", external=False)

    customer >> Relationship("Modifica las opciones") >>interfazSimple
    interfazSimple>>Relationship("Recive informacion y devuelve informacion") >> Controlador
    Controlador >> Relationship("Modifica los elementos fisicos") >> Sistema

print("Diagramas hechos")