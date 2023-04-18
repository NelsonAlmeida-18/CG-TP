import math
import sys
import os

distanceScale = 1e-6
planetScale = 1e-3
auScale = 6.68458712e-9


def solarSystemGenerator():
    scale=1
    filename="solarSystem.xml"
    if (sys.argv[1]):
        scale=float(sys.argv[1])
    if (sys.argv[2]):
        filename=sys.argv[2]

        
    xmlHeader = f"""
    <world>
        <window width="512" height="512" />
        <camera>
            <position x="10" y="10" z="10" />
            <lookAt x="0" y="0" z="0" />
            <up x="0" y="1" z="0" />
            <projection fov="60" near="1" far="1000" />
        </camera>
        <lights>
            <light type="point" posX="0" posY="10" posZ="0" />
            <light type="directional" dirX="1" dirY="1" dirZ="1" />
            <light type="spotlight" posX="0" posY="10" posZ="0" dirX="1" dirY="1" dirZ="1" cutoff="45" />
        </lights>
        <group>
    """

    finalXML = "    </group>\n</world>"

    planets = [
    {
        'name': 'Sun',
        'mass': 1.989e30,     # kg
        'radius': 695508,     # km
        'distance': 0         # km
    },
    {
        'name': 'Mercury',
        'mass': 3.3011e23,
        'radius': 2439.7,
        'distance': 57909100
    },
    {
        'name': 'Venus',
        'mass': 4.8675e24,
        'radius': 6051.8,
        'distance': 108208000
    },
    {
        'name': 'Earth',
        'mass': 5.9724e24,
        'radius': 6371,
        'distance': 149598261
    },
    {
        'name': 'Mars',
        'mass': 6.4171e23,
        'radius': 3389.5,
        'distance': 227943824
    },
    {
        'name': 'Jupiter',
        'mass': 1.8982e27,
        'radius': 69911,
        'distance': 778340821
    },
    {
        'name': 'Saturn',
        'mass': 5.6834e26,
        'radius': 58232,
        'distance': 1426666422
    },
    {
        'name': 'Uranus',
        'mass': 8.6810e25,
        'radius': 25362,
        'distance': 2870658186
    },
    {
        'name': 'Neptune',
        'mass': 1.0243e26,
        'radius': 24622,
        'distance': 4498396441
    }
    ]


    for x in planets:
        #if x["name"] == "Saturn":
        #    generatePlanet(x["name"], scale*planetScale*x["radius"])
        #    generateRing(planetScale*scale*["earth"]*21, planetScale*scale*astros["earth"]*16.5)
        #    xmlHeader+=generateTransformations(x, scale*distanceScale*distances[x], 1)
        if x["name"] == "Sun":
            generatePlanet(x["name"], scale*planetScale*x["radius"])
            xmlHeader+=generateTransformations(x["name"], scale*distanceScale*x["distance"], 0, 1)
        elif x["name"] == "Saturn":
            generatePlanet(x["name"], scale*planetScale*x["radius"])
            generateRing(planetScale*scale*6371*21, planetScale*scale*6371*8)
            xmlHeader+=generateTransformations(x["name"], scale*distanceScale*x["distance"]+(695508*scale*planetScale), 1, 1)
        else:
            generatePlanet(x["name"], scale*planetScale*x["radius"])
            xmlHeader+=generateTransformations(x["name"], scale*distanceScale*x["distance"]+(695508*scale*planetScale), 0, 1)

    xmlHeader+=finalXML
    file = open(filename, "w")
    file.write(xmlHeader)
    file.close()
    print("Solar system generated!")


def generateTransformations(name,distance,torus, scale):
    template = f"""
    <group>
        <transform>
            <translate x="{distance}" y="0" z="0" />
            <scale x="{1}" y="{1}" z="{1}" />
        </transform>
        <models>
            <model file="solarSystem/{name}.3d" />
        </models>
    </group>
    """

    if torus:
        template += f"""
        <group>
            <transform>
                <translate x="{distance}" y="0" z="0" />
                <scale x="{1}" y="{1}" z="{1}" />
                <rotate angle="60" x="1" y="0" z="0" />
                <rotate angle="40" x="0" y="0" z="1" />
            </transform>
            <models>
                <model file="solarSystem/torus.3d" />
            </models>
        </group>
        """

    return template


def generateRing(outer_r, inner_r):
    #step 1 find if generator in path
    if "generator" in os.listdir():
        if "solarSystem" not in os.listdir("../3d/"):
            os.system("mkdir ../3d/solarSystem")
        
        command = f"./generator torus {outer_r} {inner_r} 0.5 30 30 ../3d/solarSystem/torus.3d"
        os.system(command)
    else:
        if "generator.cpp" in os.listdir():
            os.system("g++ generator.cpp -o generator")
            generateRing(outer_r, inner_r)
        else:
            print("Could not find a generator")


def generatePlanet(planetName, dimensions):
    #step 1 find if generator in path
    if "generator" in os.listdir():
        if "solarSystem" not in os.listdir("../3d/"):
            os.system("mkdir ../3d/solarSystem")
        command = f"./generator sphere {dimensions} {30} {30} ../3d/solarSystem/{planetName}.3d"
        os.system(command)
    else:
        if "generator.cpp" in os.listdir():
            os.system("g++ generator.cpp -o generator")
            generatePlanet(planetName, dimensions)
        else:
            print("Could not find a generator")

solarSystemGenerator()