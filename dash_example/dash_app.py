import dash
import numpy as np
from dash import Input, Output, dcc, html
import plotly.express as px
import pandas as pd
from datetime import datetime
from plotly.subplots import make_subplots
import plotly.graph_objects as go

app = dash.Dash(__name__)

app.layout = html.Div([
        html.H1("Modelador simple de las ventas historicas del año movil"),
        html.Tr([html.Td(["Incremento de pzs vendidas:"]),html.Td([ dcc.Input(id='lambda', value=1, type='text')])]),
        html.Tr([html.Td(["Incremento del precio: "]),html.Td([ dcc.Input(id='lambda', value=1, type='text')])]),
    html.Hr(),
    html.Tr([html.Td(html.H3(id='value'),className="expanded"), html.Td(html.H3(id='units'))]),
    html.Div([dcc.Graph(id='histogram')],className= "container forward")
    ],className="container animated")


@app.callback(
   Output("histogram", "figure"),
    Output("value", "children"),
    Output("units", "children"),
    Input("lambda", "value"),
)
def graph_histogram(lambda_value):
    if lambda_value != "":
        s = pd.date_range(end = datetime.today(), periods = 20).to_pydatetime().tolist()

        sales = np.random.default_rng().uniform(1000,1200,20)
        sales_2 = np.random.default_rng().uniform(1000,1200,20)

        for index,item in enumerate(sales):
            sales[index] = item * float(lambda_value)
            #print(lambda_value)
        fig =make_subplots(rows=1, cols=1)
        fig.append_trace(go.Scatter(x=s,y=sales,marker= dict(
                color="#9e2f7f"
                )),1,1)
        fig.append_trace(go.Scatter(x=s,y=sales_2,marker= dict(
                color="#e75263"
                )),1,1)
        fig.update_layout(title="Ventas por mes",
                xaxis_title="Fecha",
                yaxis_title="Venta",
                autosize=False,
                width=1400,
                height=500,
                paper_bgcolor="#1d2228",
                plot_bgcolor="#1d2228",
                        font=dict(
                            family="Courier New, monospace",
                                size=18,
                            color="#fd9668"
        ))
        fig.update_xaxes(showline=False, linewidth=2, gridcolor='#49bf9d')
        fig.update_yaxes(showline=False, linewidth=2, gridcolor='#49bf9d')
        return fig, f'Margen anual: {2}',f'Pzs extras a vender: {2}'
    else:
        return dash.no_update


if __name__ == "__main__":
    app.run_server(debug=True)