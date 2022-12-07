import dash
import numpy as np
from dash import Input, Output, dcc, html
import plotly.express as px
import dash_bootstrap_components as dbc

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



app.layout = dbc.Container(
    [
        html.H1("Poisson distribution graph"),
        html.Hr(),
        dbc.Row(dbc.Col(inputs)),
        dbc.Row(dbc.Col(graph)),
    ]
)


@app.callback(
    Output("histogram", "figure"),
    Input("lambda", "value"),
)
def graph_histogram(lambda_value):
    if lambda_value:
        s = range(0,lambda_value)
        axis_x = np.linspace(2.0, 3.0, num=lambda_value)
        fig = px.line(x=axis_x, y=s)
        return fig
    else:
        return dash.no_update


if __name__ == "__main__":
    app.run_server(debug=True)