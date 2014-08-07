/*
 *    Copyright 2013, 2014 Ondrej Navratil.
 *    This project and all of its contents are distributed under the terms of the GNU General Public License.
 *
 *    This file is part of CPNSimulator.
 *
 *    CPNSimulator is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published b
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    CPNSimulator is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with CPNSimulator. If not, see <http://www.gnu.org/licenses/>.
*/

#include "arc.h"
#include "computer.h"
#include "analyzer.h"

Arc::Arc() :
    parsedExpression(NULL)
{
}

Arc::~Arc()
{
    if(parsedExpression)
        delete parsedExpression;
}

QList<Binding> Arc::findBindings(Computer *computer, NetMarking marking)
{
    QList<Binding> bindings;
    int left = eval(parsedExpression->left, NULL, NULL, computer).value.i; // no tables needed - no ID's allowed
    if(parsedExpression->right->type == Expression::DATA)
    {
        if(place->colourSet == Place::UNIT)
        {
            if(parsedExpression->right->data->value.b == true) //right == unit
            {
                if(marking[place].value.multiUnit >= left)
                    bindings.append(Binding());
            }
            else    //right == nounit
            {
                bindings.append(Binding());
            }
        }
        else if(place->colourSet == Place::BOOL)
        {
            if(parsedExpression->right->data->value.b == true)
            {
                if(marking[place].value.multiBool.t >= left)
                    bindings.append(Binding());
            }
            else
            {
                if(marking[place].value.multiBool.f >= left)
                    bindings.append(Binding());
            }
        }
        else if(place->colourSet == Place::INT)
        {
            int right = parsedExpression->right->data->value.i;
            if(marking[place].value.multiInt->value(right, 0) >= left)
                bindings.append(Binding());
        }
    }
    else
    {
        //there is no variable of dataType unit
        if(place->colourSet == Place::BOOL)
        {
            if(marking[place].value.multiBool.t >= left)
            {
                Binding binding;
                BindingElement element(parsedExpression->right->id, Data::BOOL);
                element.data().value.b = true;
                binding.append(element);
                bindings.append(binding);
            }

            if(marking[place].value.multiBool.f >= left)
            {
                Binding binding;
                BindingElement element(parsedExpression->right->id, Data::BOOL);
                element.data().value.b = false;
                binding.append(element);
                bindings.append(binding);
            }

        }
        else if(place->colourSet == Place::INT)
        {
            QMapIterator<int, int>i(*marking[place].value.multiInt);
            while(i.hasNext())
            {
                i.next();
                if(i.value() < left)
                    continue;
                Binding binding;
                BindingElement element(parsedExpression->right->id, Data::INT);
                element.data().value.i = i.key();
                binding.append(element);
                bindings.append(binding);
            }

        }
    }
    return bindings;
}
