import dash
import numpy as np
from dash import Input, Output, dcc, html
import plotly.express as px
import dash_bootstrap_components as dbc
import pandas as pd
from datetime import datetime
from plotly.subplots import make_subplots
import plotly.graph_objects as go

app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP])


inputs = dbc.Card(
    dbc.CardBody(
        [
            html.P(
                "lambda (must be >= 0): ",
                style={"display": "inline-block", "margin-right": "8px"},
            ),
            dcc.Input(
                id="lambda", placeholder="number", value=4
            ),
        ]
    )
)

# Histogram
graph = dbc.Card(dbc.CardBody(dcc.Graph(id="histogram")))
graph2 = dbc.Card(dbc.CardBody(dcc.Graph(id="fig_2")))

app.layout = dbc.Container(
    [
        html.H1("Poisson distribution graph"),
        html.Hr(),
        dbc.Row(dbc.Col(inputs)),
        dbc.Row(dbc.Col(graph)),
        dbc.Col(graph2),
    ]
)


@app.callback(
   [ Output("histogram", "figure"),Output('fig_2' , 'figure')],
    Input("lambda", "value"),
)
def graph_histogram(lambda_value):
    if lambda_value:
        s = pd.date_range(end = datetime.today(), periods = 20).to_pydatetime().tolist()

        sales = np.random.default_rng().uniform(1000,1200,20)
        sales_2 = np.random.default_rng().uniform(1000,1200,20)

        for index,item in enumerate(sales):
            sales[index] = item * int(lambda_value)
            #print(lambda_value)
        fig =make_subplots(rows=2, cols=1)
        fig.append_trace(go.Scatter(x=s,y=sales),1,1)
        fig.append_trace(go.Scatter(x=s,y=sales_2),1,1)
        fig_2 = px.line(x=sales, y=s)
        return [fig, fig_2]
    else:
        return dash.no_update


if __name__ == "__main__":
    app.run_server(debug=True)