/*!
 *  \file PanelManager.cpp
 *  \brief
 *    Provides an example of how to create command panels.
 *  \author Michael Plooster
 *  \date 11 Feb 2014
 */

#include "PanelManager.hpp"
#include "Claro.hpp"
#include "NavigationModel.hpp"
#include "NavigationNode.hpp"
#include "MyWidgetFactory.hpp"

#include <QAction>
#include <QStringList>

PanelManager::PanelManager() :
  isInitialized(false),
  myFactory(NULL)
{}

PanelManager::~PanelManager()
{
  if(myFactory)
    delete myFactory;
}

void PanelManager::initialize()
{
  if(isInitialized)
    return;

  // Set this variable to choose how to initialize your navigation nodes.
  bool use_xml = false;

  if(use_xml)
    initialize_from_xml();
  else
    initialize_from_code();

  // Once nodes are created and initialized, we want to associate them
  // with command panels.
  associate_panels_with_nodes();
  isInitialized = true;
}

void PanelManager::clear()
{
  if(myFactory)
  {
    // Clean up my widgets
    myFactory->clear();

    // Remove the factory reference from my navigation nodes
    QStringList my_markers;
    my_markers.push_back("MySecondLevelNode1");
    my_markers.push_back("MySecondLevelNode2");

    // For each marker, we want to get the navigation node and assign the node
    // to use this factory to get widgets as needed.
    NavigationModel* model = Claro::instance()->navigation_model();
    for(int i = 0; i < my_markers.size(); i++)
    {
      NavigationNode* node = model->getMarkedNode(my_markers[i]);
      if(node)
        node->setFactory(0);
    }
  }

  isInitialized = false;
}

void PanelManager::initialize_from_xml()
{
  // If you are creating your command panel navigation buttons using xml files,
  // there is nothing you need to do to create navigation nodes (it is done
  // automatically). For an example, see MyNavigation.xml and copy it to the
  // Trelis/bin/xml folder.

  return;
}

void PanelManager::initialize_from_code()
{
  // This example shows how to create the command panel navigation buttons (nodes)
  // using the NavigationModel directly.
  NavigationModel* model = Claro::instance()->navigation_model();
  if(!model)
    return;

  // Create the top-level node (which will be next to Geometry, Mesh, etc.)
  NavigationNode* root_node = model->getMarkedNode("MyRootNode");
  if(!root_node)
  {
    // Add a node and set it's parent to the model's root so it will be top-level.
    root_node = model->addNode("MyRootNodeName", model->getRoot());

    // Set the title that will be displayed for nodes under this node.
    root_node->setTitle("MyActions");

    // Set the marker so that we can find this node later if we want. Marker
    // names should be unique.
    model->setNodeMarker(root_node, "MyRootNode");
  }

  // Create our second level nodes (below our root node)
  NavigationNode* node = model->getMarkedNode("MySecondLevelNode1");
  if(!node)
  {
    // Add a node and set it's parent to our component's root node.
    node = model->addNode("MySecondLevelNode1Name", root_node);
    model->setNodeMarker(node, "MySecondLevelNode1");
  }

  node = model->getMarkedNode("MySecondLevelNode2");
  if(!node)
  {
    // Add a node and set it's parent to our component's root node.
    node = model->addNode("MySecondLevelNode2Name", root_node);
    model->setNodeMarker(node, "MySecondLevelNode2");
  }
}

void PanelManager::associate_panels_with_nodes()
{

  // The widget factory is in charge of giving out our panels when the framework
  // needs them.
  if(!myFactory)
    myFactory = new MyWidgetFactory();

  // Setup a list of the markers that identify my navigation nodes. Note that
  // we did not include our root marker because we do not need to create a
  // command panel for it.
  QStringList my_markers;
  my_markers.push_back("MySecondLevelNode1");
  my_markers.push_back("MySecondLevelNode2");

  // For each marker, we want to get the navigation node and assign the node
  // to use this factory to get widgets as needed.
  NavigationModel* model = Claro::instance()->navigation_model();
  for(int i = 0; i < my_markers.size(); i++)
  {
    NavigationNode* node = model->getMarkedNode(my_markers[i]);
    if(node)
      node->setFactory(myFactory);
  }

}
