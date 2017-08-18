//
//  main.cpp
//  neuralNetwork
//
//  Created by Эльдар Дамиров on 18.08.17.
//  Copyright © 2017 Эльдар Дамиров. All rights reserved.
//

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <random>
#include <assert.h>
#include <cmath>


class neuron;


typedef std::vector <neuron> layer;

struct connection
    {
    double weight;
    double deltaWeight;
    };


class neuron
    {
    public:
        neuron ( unsigned numberOfOutputs, unsigned myIndex );
        void feedForward ( const layer &previousLayer, unsigned myIndex );

        double getOutputValue() const
            {
            return outputValue;
            }

        void setOutputValue ( double newValue )
            {
            outputValue = newValue;
            }


    private:
        double outputValue;
        unsigned myIndex;
        std::vector <connection> outputWeights;

        double randomWeight ()
            {
            return ( rand() / double ( RAND_MAX ) );
            }


        static double transferFunction ( double x );
        static double transferFunctionDerivative ( double x );


    };







class net
    {
    public:
        net ( const std::vector <unsigned> &topology );

        void feedForward ( const std::vector <double> &inputValues );
        void backProp ( const std::vector <double> &targetValues );  // learning;
        void getResults ( std::vector <double> &resultValues ) const {};


    private:
        std::vector <layer> layers;



        


    };


net::net ( const std::vector <unsigned> &topology )
    {
    unsigned long numberOfLayers = topology.size();

    for ( unsigned currentLayer = 0; currentLayer < numberOfLayers; currentLayer++ )
        {
        layers.push_back ( layer() );
        unsigned numberOfOutputs = currentLayer == ( topology.size() - 1 ) ? 0 : topology [ currentLayer + 1 ];

        for ( unsigned currentNeuronInLayer = 0; currentNeuronInLayer <= topology [ currentLayer ]; currentNeuronInLayer++ ) // <= because of bias neuron;
            {
            layers.back().push_back ( neuron ( numberOfOutputs, 0 ) );
//            std::cout << "Hello I'm new neuron!";
            }
        }
    }







int main()
    {

    std::vector <unsigned> topology;

    //
    topology.push_back ( 3 );
    topology.push_back ( 2 );
    topology.push_back ( 1 );

    //


    net myNet ( topology );

    //


    //

    std::vector <double> inputValues;
    myNet.feedForward ( inputValues );

    std::vector <double> targetValues;
    myNet.backProp ( targetValues );

    std::vector <double> resultValues;
    myNet.getResults ( resultValues );





    return 0;
    }




void net::feedForward ( const std::vector<double> &inputValues )
    {
    assert ( inputValues.size() == ( layers [ 0 ].size() - 1 ) );

    for ( unsigned input = 0; input < inputValues.size(); input++ )
        {
        layers [ 0 ] [ input ].setOutputValue ( inputValues [ input ] );
        }


    for ( unsigned currentLayer = 1; currentLayer < layers.size(); currentLayer++ )
        {
        layer &previousLayer = layers [ currentLayer - 1 ];
        for ( unsigned currentNeuron = 0; currentNeuron < layers [ currentLayer ].size() - 1; currentNeuron++ )
            {
            layers [ currentLayer ] [ currentNeuron ].feedForward ( previousLayer , 1000000 );
            }
        }
    }


void net::backProp ( const std::vector<double> &targetValues )
    {
    layer &outputLayer = layers.back(); // all error in all net;

    double sumError = 0.0;
    double delta = 0.0;
    for ( unsigned currentNeuron = 0; currentNeuron < ( outputLayer.size() - 1 ); currentNeuron++ )
        {
        delta = targetValues [ currentNeuron ] - outputLayer [ currentNeuron ].getOutputValue();
        sumError = sumError + ( delta * delta );
        }

    sumError = sumError / ( outputLayer.size() - 1 );

    }


////////




neuron::neuron ( unsigned numberOfOutput, unsigned myIndex )
    {
    for ( unsigned connections = 0; connections < numberOfOutput; connections++ )
        {
        outputWeights.push_back ( connection() );
        outputWeights.back().weight = randomWeight();
        }

    myIndex = myIndex;


    }

void neuron::feedForward ( const layer &previousLayer, unsigned myIndex )
    {
    double sum = 0.0;

    for ( unsigned previousLayerNeuron = 0; previousLayerNeuron < previousLayer.size(); previousLayerNeuron++ )
        {
        sum = sum + previousLayer [ previousLayerNeuron ].getOutputValue() * previousLayer [ previousLayerNeuron ].outputWeights [ myIndex ].weight;
        }

    outputValue = neuron::transferFunction ( sum );
    }


double neuron::transferFunction ( double x )
    {
    // [ - 1.0 ... 1.0 ];

    return tanh ( x );
    }

double neuron::transferFunctionDerivative ( double x )
    {
    return ( 1.0 - ( x * x ) );
    }
