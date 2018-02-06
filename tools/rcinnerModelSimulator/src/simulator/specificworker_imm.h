// ------------------------------------------------------------------------------------------------
// InnerModelManager.ice
// ------------------------------------------------------------------------------------------------
//#define INNERMODELMANAGERDEBUG

// Moves item to the position defined by pose respect to the base
bool SpecificWorker::imm_setPose(const QString &server, const std::string &base, const std::string &item, const RoboCompInnerModelManager::Pose3D &pose)
{
	InnerModelMgr::guard gl(innerModel.mutex());
	
	QString qBase = QString::fromStdString(base);
	QString qItem = QString::fromStdString(item);
	QString m="RoboCompInnerModelManager::setPose()";

	//check type transform
	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qBase, m));
	checkOperationInvalidNode(aux, m + qBase +"can't be use as base because it's not a InnerModelTransform node.");
	aux = NULL;
	aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qItem, m));
	checkOperationInvalidNode(aux, m + qItem +"can't be use as item because it's not a InnerModelTransform node.");

	innerModel->updateTransformValues(qItem, pose.x, pose.y, pose.z, pose.rx, pose.ry, pose.rz, qBase);

	return true;
}


bool SpecificWorker::imm_setPoseFromParent(const QString &server, const std::string &item, const RoboCompInnerModelManager::Pose3D &pose)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString qItem = QString::fromStdString(item);
	QString m="RoboCompInnerModelManager::setPose()";

	//check type transform
	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qItem, m));
	checkOperationInvalidNode(aux, m + qItem +"can't be use as item because it's not a InnerModelTransform node.");
	innerModel->updateTransformValues(qItem, pose.x, pose.y, pose.z, pose.rx, pose.ry, pose.rz);

	return true;
}


/// ---------------------------------------------------------------------------------------
// Provides the pose of a certain item respect to the base.
/// ---------------------------------------------------------------------------------------
bool SpecificWorker::imm_getPose(const QString &server, const std::string &base, const std::string &item, RoboCompInnerModelManager::Pose3D &pose)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QVec p;
	QString qBase = QString::fromStdString(base);
	QString qItem = QString::fromStdString(item);
	QString m="RoboCompInnerModelManager::getPose()";

	// check type transform
	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qBase, m));
	checkOperationInvalidNode(aux, m + qBase +"can't be use as base because it's not a InnerModelTransform node.");
	aux = NULL;
	aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qItem, m));
	checkOperationInvalidNode(aux, m + qItem +"can't be use as item because it's not a InnerModelTransform node.");

	// calculate position
	p = innerModel->transform(QString::fromUtf8(base.c_str()), QVec::vec3(0,0,0), QString::fromUtf8(item.c_str()));
	pose.x = p.x();
	pose.y = p.y();
	pose.z = p.z();
	//calulate rotation
	p = innerModel->getRotationMatrixTo(QString::fromUtf8(base.c_str()), QString::fromUtf8(item.c_str())).extractAnglesR();
	pose.rx = p.x();
	pose.ry = p.y();
	pose.rz = p.z();

	return true;
}


// Provides the pose of a certain item respect to the parent
bool SpecificWorker::imm_getPoseFromParent(const QString &server, const std::string &item, RoboCompInnerModelManager::Pose3D &pose)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString m="RoboCompInnerModelManager::getPoseFromParent()";

	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(QString::fromStdString(item), m));
	checkOperationInvalidNode(aux, m + aux->getId() + "can't be used as base because it's not a InnerModelTransform node.");

	pose.x = aux->getBacktX();
	pose.y = aux->getBacktY();
	pose.z = aux->getBacktZ();
	pose.rx = aux->getBackrX();
	pose.ry = aux->getBackrY();
	pose.rz = aux->getBackrZ();

	return true;
}


// Provides the transform of a certain point expressed in Base to Item.
bool SpecificWorker::imm_transform(const QString &server, const std::string &base, const std::string &item, const RoboCompInnerModelManager::coord3D &coordInItem, RoboCompInnerModelManager::coord3D &coordInBase)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QVec p;
	const QString qBase = QString::fromStdString(base);
	const QString qItem = QString::fromStdString(item);
	const QString m="RoboCompInnerModelManager::transform()";

	//check type transform
	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qBase, m));
	checkOperationInvalidNode(aux, m + qBase +"can't be used as base because it's not a InnerModelTransform node.");

	aux = NULL;
	aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qItem, m));
	checkOperationInvalidNode(aux, m + qItem +"can't be used as item because it's not a InnerModelTransform node.");

	// 	QString qBase = QString::fromStdString(base);
// 	QString qItem = QString::fromStdString(item);
// 	QString m="RoboCompInnerModelManager::transform()";
// 
// 	//check type transform
// 	InnerModelTransform *aux = dynamic_cast<InnerModelTransform*>(getNode(qBase, m));
// 	checkOperationInvalidNode(aux, m + qBase +"can't be use as base because it's not a InnerModelTransform node.");
// 
// 	aux = NULL;
// 	aux = dynamic_cast<InnerModelTransform*>(getNode(qItem, m));
// 	checkOperationInvalidNode(aux, m + qItem +"can't be use as item because it's not a InnerModelTransform node.");

	// calculate position
	p = innerModel->transform(QString::fromUtf8(base.c_str()), QVec::vec3(coordInItem.x,coordInItem.y,coordInItem.z),QString::fromUtf8(item.c_str()));
	coordInBase.x = p.x();
	coordInBase.y = p.y();
	coordInBase.z = p.z();

	return true;
}

RoboCompInnerModelManager::Matrix SpecificWorker::imm_getTransformationMatrix(const std::string &item, const std::string &base)
{
// 	qFatal("Not well implemented yet.");
	const QString qBase = QString::fromStdString(base);
	const QString qItem = QString::fromStdString(item);
	const QString m="RoboCompInnerModelManager::transform()";
	InnerModelMgr::guard gl(innerModel.mutex());


	//check type transform
	InnerModel::TransformPtr aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qBase, m));
	checkOperationInvalidNode(aux, m + qBase +"can't be used as base because it's not a InnerModelTransform node.");

	aux = NULL;
	aux = std::dynamic_pointer_cast<InnerModelTransform>(getNode(qItem, m));
	checkOperationInvalidNode(aux, m + qItem +"can't be used as item because it's not a InnerModelTransform node.");

	// calculate position
	RTMat retA = innerModel->getTransformationMatrix(qBase, qItem);
	RoboCompInnerModelManager::Matrix retB;

	retB.cols = retA.nCols();
	retB.rows = retA.nRows();
	retB.data.resize(retB.cols*retB.rows);
	retA.print("retA");
	printf("\n");
	for (int r=0; r<retB.rows; r++)
	{
		for (int c=0; c<retB.cols; c++)
		{
			printf("%f ", retB.data[r*retB.cols + c]);
			retB.data[r*retB.cols + c] = retA(r, c);
		}
		printf("\n");
	}
	printf("\n");

	return retB;
}


// Scales a mesh to a new size.
bool SpecificWorker::imm_setScale(const QString &server, const std::string &item, float scaleX, float scaleY, float scaleZ)
{
// 	InnerModelMgr::guard gl(innerModel.mutex());

// 	QString qItem = QString::fromStdString(item);
// 	QString m="RoboCompInnerModelManager::setScale()";
// 
// 	InnerModelMesh *aux = dynamic_cast<InnerModelMesh*>(getNode(QString::fromStdString(item),m));
// 	checkOperationInvalidNode(aux, m + qItem +"can't be used because it's not a InnerModelMesh node.");
// 
// 	aux->setScale(scaleX, scaleY, scaleZ);
// 	innerModel->update();
// 
// #ifdef INNERMODELMANAGERDEBUG
// 	try {
// 		checkPoseCollision(qItem,m);
// 	} catch(RoboCompInnerModelManager::InnerModelManagerError err) {
// 		std::cout<<err.what() <<" "<<err.text<< "\n";
// 		std::cout<< "\n";
// 		///come back to t= (t+1) -t
// 
// 		//to check => maybe using a tag in the xml (collide="true") to decide if allow collitions or not
// 		//  innerModel->updateTransformValues(qItem,p.x, p.y, p.z, p.rx , p.ry, p.rz);
// 		//  innerModel->update();
// 		throw err;
// 	}
// #endif

	InnerModelMgr::guard gl(innerModel.mutex());

	QString qItem = QString::fromStdString(item);
	QString m="RoboCompInnerModelManager::setScale()";

	InnerModel::MeshPtr aux = std::dynamic_pointer_cast<InnerModelMesh>(getNode(QString::fromStdString(item),m));
	checkOperationInvalidNode(aux,m + qItem +"can't be used because it's not a InnerModelMesh node.");

	aux->setScale(scaleX, scaleY, scaleZ);
	//I think not necessary	
	// 	innerModel->update();

#ifdef INNERMODELMANAGERDEBUG
// 	try {
// 		checkPoseCollision(qItem,m);
// 	} catch(RoboCompInnerModelManager::InnerModelManagerError err) {
// 		std::cout<<err.what() <<" "<<err.text<< "\n";
// 		std::cout<< "\n";
// 		///come back to t= (t+1) -t
// 
// 		//to check => maybe using a tag in the xml (collide="true") to decide if allow collitions or not
// 		//  innerModel->updateTransformValues(qItem,p.x, p.y, p.z, p.rx , p.ry, p.rz);
// 		//  innerModel->update();
// 		throw err;
// 	}
#endif

	return true;
}


bool SpecificWorker::imm_setPlane(const QString &server, const std::string &item, const RoboCompInnerModelManager::Plane3D &plane)
{	
	InnerModelMgr::guard gl(innerModel.mutex());

	QString m="RoboCompInnerModelManager::setPlane()";
	InnerModel::PlanePtr aux = dynamic_pointer_cast<InnerModelPlane>(getNode(QString::fromStdString(item), m));
	checkOperationInvalidNode(aux,m + aux->getId() +"can't be use as base because it's not of the type InnerModelPlane.");
	innerModel->updatePlaneValues(QString::fromStdString(item), plane.nx, plane.ny, plane.nz, plane.px, plane.py, plane.pz);
	return true;
}

bool SpecificWorker::imm_setPlaneTexture(const QString &server, const std::string &item, const std::string &texture)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString m="RoboCompInnerModelManager::setPlaneTextureº()";
	printf("SETPLANETEXTURE %s: %s\n", item.c_str(), texture.c_str());
	InnerModel::PlanePtr aux = std::dynamic_pointer_cast<InnerModelPlane>(getNode(QString::fromStdString(item), m));
	qDebug() << "aux->texture" << aux->getTexture() << "qstring" << QString::fromStdString(texture);
	
	aux->setTexture(QString::fromStdString(texture));
	
	osg::Image *image=NULL;
	image = osgDB::readImageFile(texture);
	if (not image)
	{
		qDebug() << "Couldn't load texture:" << texture.c_str();
		throw "Couldn't load texture.";
	}
	
	imv->planesHash[aux->getId()]->image =image;
	imv->planesHash[aux->getId()]->texture->setImage(image);

	qDebug() << "change aux->texture" << aux->getTexture();
// 	checkOperationInvalidNode(aux,m + aux->getId() +"can't be use as base because it's not of the type InnerModelPlane.");
// 	innerModel->updatePlaneValues(QString::fromStdString(item), plane.nx, plane.ny, plane.nz, plane.px, plane.py, plane.pz);
	return true;
}



// void SpecificWorker::addPointCloud(const std::string &id)
// {
// }


void SpecificWorker::imm_setPointCloudData(const QString &server, const std::string &id, const RoboCompInnerModelManager::PointCloudVector &cloud)
{
// 	QString m = QString("SpecificWorker::setPointCloudData");
// 
// 	std::cout<<"setPointCloudData: "<<id<<" "<<cloud.size() <<std::endl;
// 
// 	/// Aqui Marco va a mejorar el código :-) felicidad (comprobar que la nube existe)
// 	IMVPointCloud *pcNode = imv->pointCloudsHash[QString::fromStdString(id)];
// 
// 	int points = cloud.size();
// 	pcNode->points->resize(points);
// 	pcNode->colors->resize(points);
// 	pcNode->setPointSize(1);
// 	for(int i=0 ; i<points; i++) {
// 		pcNode->points->operator[](i) = QVecToOSGVec(QVec::vec3(cloud[i].x, -cloud[i].y, cloud[i].z));
// 		pcNode->colors->operator[](i) = osg::Vec4f(float(cloud[i].r) /255, float(cloud[i].g) /255, float(cloud[i].b) /255, 1.f);
// 	}
// 	pcNode->update();
// 	imv->update();
	
	QString m = QString("SpecificWorker::setPointCloudData");

	std::cout<<"setPointCloudData: "<<id<<" "<<cloud.size() <<std::endl;

	/// Aqui Marco va a mejorar el código :-) felicidad (comprobar que la nube existe)
	IMVPointCloud *pcNode = imv->pointCloudsHash[QString::fromStdString(id)];

	int points = cloud.size();
	pcNode->points->resize(points);
	pcNode->colors->resize(points);
	pcNode->setPointSize(1);
	for(int i=0 ; i<points; i++) {
		pcNode->points->operator[](i) = QVecToOSGVec(QVec::vec3(cloud[i].x, cloud[i].y, cloud[i].z));
		pcNode->colors->operator[](i) = osg::Vec4f(float(cloud[i].r) /255, float(cloud[i].g) /255, float(cloud[i].b) /255, 1.f);
	}
	pcNode->update();
//I think not necessary	
// 	imv->update();
}


bool SpecificWorker::imm_addTransform(const QString &server, const std::string &item, const std::string &engine, const std::string &base, const RoboCompInnerModelManager::Pose3D &pose)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	InnerModel::NodePtr parent = getNode(QString::fromStdString(base), "RoboCompInnerModelManager::addTransform()");
	checkNodeAlreadyExists(QString::fromStdString(item), "RoboCompInnerModelManager::addTransform()");
	
	QString qEngine = QString::fromStdString( engine);
	if (qEngine !="static" and qEngine !="bullet")
	{
		qEngine = "static";
	}

	InnerModel::TransformPtr tr = innerModel->newNode<InnerModelTransform>(QString::fromStdString(item), QString::fromStdString("static"), pose.x, pose.y, pose.z, pose.rx, pose.ry, pose.rz, 0, parent);
	parent->addChild(tr);
	imv->recursiveConstructor(tr, imv->mts[parent->getId()], imv->mts, imv->meshHash); // imv->osgmeshes,imv->osgmeshPats);
	
#ifdef INNERMODELMANAGERDEBUG
	qDebug()<<"engine"<<qEngine;
	qDebug() <<"transform: pose.x<<pose.y<<pose.z"<<pose.x<<pose.y<<pose.z<<QString::fromStdString(item);
#endif
	return true;
}


bool SpecificWorker::imm_addJoint(const QString &server, const std::string &item, const std::string &base, const RoboCompInnerModelManager::jointType &j_)
{
	RoboCompInnerModelManager::jointType j = j_;
	if (j.axis == "")
	{
		j.axis = "z";
	}

	InnerModelMgr::guard gl(innerModel.mutex());

	RoboCompInnerModelManager::Pose3D pose = j.pose;

	InnerModel::TransformPtr parent = std::dynamic_pointer_cast<InnerModelTransform>(getNode(QString::fromStdString(base), "RoboCompInnerModelManager::addJoint()"));
	checkNodeAlreadyExists(QString::fromStdString(item), "RoboCompInnerModelManager::addJoint()");

	InnerModel::JointPtr jN = innerModel->newNode<InnerModelJoint>(QString::fromStdString(item), j.lx, j.ly, j.lz, j.hx, j.hy, j.hz, pose.x, pose.y, pose.z, pose.rx, pose.ry, pose.rz, j.min, j.max, j.port, j.axis, 0, parent);
	parent->addChild (jN);

	// Create Interface in case the port is not 0
	if (jN->getPort() != 0)
		addJM(jN);

	imv->recursiveConstructor(jN, imv->mts[parent->getId()], imv->mts, imv->meshHash); // imv->osgmeshes,imv->osgmeshPats);

	return true;
}


bool SpecificWorker::imm_addMesh(const QString &server, const std::string &item, const std::string &base, const RoboCompInnerModelManager::meshType &m)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString msg="RoboCompInnerModelManager::addMesh()";
#ifdef INNERMODELMANAGERDEBUG
	qDebug() <<msg<<QString::fromStdString(base) <<QString::fromStdString(item);
	qDebug() <<QString::fromStdString(m.meshPath);
#endif
	InnerModel::TransformPtr parent = std::dynamic_pointer_cast<InnerModelTransform>(getNode(QString::fromStdString(base), msg));

	//Checking if its parent is not a mesh.
	checkOperationInvalidNode(parent, msg);
	checkNodeAlreadyExists(QString::fromStdString(item), msg);
	checkInvalidMeshValues(m,msg);
	
	InnerModelMesh::RenderingModes render;
	if(m.render == 0)
		render= InnerModelMesh::RenderingModes::NormalRendering;
	if(m.render == 1)
		render= InnerModelMesh::RenderingModes::WireframeRendering;
	
	if(render != InnerModelMesh::RenderingModes::NormalRendering and render != InnerModelMesh::RenderingModes::WireframeRendering)
		render = InnerModelMesh::RenderingModes::NormalRendering;
	
	InnerModel::MeshPtr mesh = innerModel->newNode<InnerModelMesh>(
		QString::fromStdString(item),
		QString::fromStdString(m.meshPath),
		m.scaleX, m.scaleY, m.scaleZ,
		render,
		m.pose.x, m.pose.y, m.pose.z,
		m.pose.rx, m.pose.ry, m.pose.rz, false, parent);

	mesh->setScale(m.scaleX, m.scaleY, m.scaleZ);
	parent->addChild(mesh);

	imv->recursiveConstructor(mesh, imv->mts[parent->getId()], imv->mts, imv->meshHash); // osgmeshes,imv->osgmeshPats);
 	
	return true;
}


bool SpecificWorker::imm_addPlane(const QString &server, const std::string &item, const std::string &base, const RoboCompInnerModelManager::Plane3D &p)
{	
	InnerModelMgr::guard gl(innerModel.mutex());
	InnerModel::NodePtr parent = getNode(QString::fromStdString(base), "RoboCompInnerModelManager::addPlane()");
	checkNodeAlreadyExists(QString::fromStdString(item), "RoboCompInnerModelManager::addPlane()");

	InnerModel::PlanePtr plane = innerModel->newNode<InnerModelPlane>(QString::fromStdString(item), QString::fromStdString(p.texture),
	                         p.width, p.height, p.thickness, 1,
	                         p.nx, p.ny, p.nz, p.px, p.py, p.pz, false, parent);
	parent->addChild(plane);
	imv->recursiveConstructor(plane, imv->mts[parent->getId()], imv->mts, imv->meshHash);
	return true;
}


bool SpecificWorker::imm_addAttribute(const QString &server, const std::string &idNode, const std::string &name, const std::string &type, const std::string &value)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString qIdNode = QString::fromStdString(idNode);
	QString qName = QString::fromStdString(name);
	QString qType = QString::fromStdString(type);
	QString qValue = QString::fromStdString(value);
	QString m = "RoboCompInnerModelManager::addAttribute()";

	InnerModel::NodePtr node = getNode(qIdNode, m);
	AttributeAlreadyExists(node,qName,m);

	InnerModelNode::AttributeType t;
	t.type=qType;
	t.value=qValue;
	auto att = node->getAttributes();
	att.insert(qName,t);
	node->setAttributes(att);
	return true;
}


bool SpecificWorker::imm_setAttribute(const QString &server, const std::string &idNode, const std::string &name, const std::string &type, const std::string &value)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString qIdNode=QString::fromStdString(idNode);
	QString qName=QString::fromStdString(name);
	QString qType=QString::fromStdString(type);
	QString qValue=QString::fromStdString(value);

	QString m="RoboCompInnerModelManager::setAttribute()";

	InnerModel::NodePtr node = getNode(qIdNode, m);
	NonExistingAttribute(node,qName,m);

	auto att = node->getAttributes();
	att[qName].type = qType;
	att[qName].value = qValue;
	node->setAttributes(att);
	
	return true;
}


bool SpecificWorker::imm_getAttribute(const QString &server, const std::string &idNode, const std::string &name, std::string &type, std::string &value)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString qIdNode=QString::fromStdString(idNode);
	QString qName=QString::fromStdString(name);
	QString m="RoboCompInnerModelManager::getAttribute()";

	InnerModel::NodePtr node = getNode(qIdNode, m);
	NonExistingAttribute(node, qName,m);

	auto att = node->getAttributes();
	type = att[qName].type.toStdString();
	value = att[qName].value.toStdString();

	return true;
}


bool SpecificWorker::imm_removeAttribute(const QString &server, const std::string &idNode, const std::string &name)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString qIdNode=QString::fromStdString(idNode);
	QString qName=QString::fromStdString(name);
	QString m="RoboCompInnerModelManager::removeAttribute()";

	InnerModel::NodePtr node = getNode(qIdNode, m);
	NonExistingAttribute(node, qName,m);

	auto att = node->getAttributes();
	att.remove(qName);
	node->setAttributes(att);

	return true;
}


bool SpecificWorker::imm_removeNode(const QString &server, const std::string &item)
{
		InnerModelMgr::guard gl(innerModel.mutex());

	QString msg="RoboCompInnerModelManager::removeNode()";
#ifdef INNERMODELMANAGERDEBUG
	qDebug() <<msg<<QString::fromStdString(item);
#endif

	QString id =QString::fromStdString(item);
	if(id=="world" || id=="root") {
#ifdef INNERMODELMANAGERDEBUG
		qDebug() <<msg<<id<<"Can't remove root elements";
#endif
		RoboCompInnerModelManager::InnerModelManagerError err;
		err.err = RoboCompInnerModelManager::OperationInvalidNode;
		std::ostringstream oss;
		oss <<msg.toStdString() <<" error, cannot remove. Node: " <<id.toStdString();
		err.text = oss.str();
		throw err;
	}

	InnerModel::NodePtr node = getNode(QString::fromStdString(item), msg);
	checkOperationInvalidNode(node,msg);

	QStringList l;
	l.clear();
	
	innerModel->getSubTree(node,&l);
// 	qDebug()<<"----------- l.size()"<<l.size();
	///remove handlers and node
	foreach (QString n, l)
	{
		///remove handlers
#ifdef INNERMODELMANAGERDEBUG
 		qDebug()<<"remove"<<n;
#endif
		InnerModel::JointPtr jN = innerModel->getNode<InnerModelJoint>(n);
		if (jN!=NULL && jN->getPort() != 0)
		{
#ifdef INNERMODELMANAGERDEBUG		
 			qDebug()<<"remove Joint"<<n<<jN->port;
#endif
			removeJM(jN);
			
		}
		
		///remove nodes in InnerModel tree
// 		innerModel->removeNode(n);
	}
	innerModel->removeSubTree(node,&l);
	

	/// Replicate InnerModel node removals in the InnerModelViewer tree. And in handlers Lists
	foreach(QString n, l) {
		/// Replicate plane removals
		if(imv->meshHash.contains(n)) {
// 			qDebug()<<"/// Replicate meshHash removals"<<n;			
			while(imv->meshHash[n].osgmeshPaths->getNumParents() > 0)
				( imv->meshHash[n].osgmeshPaths->getParent(0))->removeChild(imv->meshHash[n].osgmeshPaths);			
			while(imv->meshHash[n].osgmeshes->getNumParents() > 0)
				( imv->meshHash[n].osgmeshes->getParent(0))->removeChild(imv->meshHash[n].osgmeshes);
			while(imv->meshHash[n].meshMts->getNumParents() > 0)	
				( imv->meshHash[n].meshMts->getParent(0))->removeChild(imv->meshHash[n].meshMts);			
				
			imv->meshHash.remove(n);
// 			meshColision.remove(n);
		}
		/// Replicate transform removals
		if(imv->mts.contains(n)) {
//			qDebug()<<"/// Replicate transform removals";//<<n<<imv->mts[n]->getNumParents();
 			while(imv->mts[n]->getNumParents() > 0) {
				(imv->mts[n]->getParent(0))->removeChild(imv->mts[n]);
 			}			
 			imv->mts.remove(n);
		}
		/// Replicate plane removals
		if(imv->planeMts.contains(n)) {
//			qDebug()<<"/// Replicate plane removals";
			while(imv->planeMts[n]->getNumParents() > 0) {
				((osg::Group *)(imv->planeMts[n]->getParent(0)))->removeChild(imv->planeMts[n]);
			}
			imv->planeMts.remove(n);
			imv->planesHash.remove(n);
		}
		
	}
// 	qDebug()<<imv->meshHash.size();
// 	qDebug()<<imv->mts.size();
// 	qDebug()<<imv->planeMts.size();
// 	qDebug()<<imv->planesHash.size();
// 	qDebug()<<innerModel->getIDKeys().size();
// 	innerModel->print();
// 	qDebug()<<"----";
//I think not necessary	
// 	innerModel->update();
// 	imv->update();
	
	return true;
}
#define INNERMODELMANAGERDEBUG
bool SpecificWorker::imm_moveNode(const QString &server, const std::string &src, const std::string &dst)
{
	InnerModelMgr::guard gl(innerModel.mutex());

	QString msg="RoboCompInnerModelManager::moveNode()";
#ifdef INNERMODELMANAGERDEBUG
	qDebug() <<msg<<QString::fromStdString(src)<<QString::fromStdString(dst);
#endif

	QString idSrc =QString::fromStdString(src);
	QString idDst =QString::fromStdString(dst);
	if(idSrc=="world" || idSrc=="root" ) {
#ifdef INNERMODELMANAGERDEBUG
		qDebug() <<msg<<idSrc<<"Can't move root elements";
#endif
		RoboCompInnerModelManager::InnerModelManagerError err;
		err.err = RoboCompInnerModelManager::OperationInvalidNode;
		std::ostringstream oss;
		oss <<msg.toStdString() <<" error, cannot move Node: " <<idSrc.toStdString();
		err.text = oss.str();
		throw err;
	}
	
	if(idDst =="root" ) {
#ifdef INNERMODELMANAGERDEBUG
		qDebug() <<msg<<idDst<<"Can't move to root elements";
#endif
		RoboCompInnerModelManager::InnerModelManagerError err;
		err.err = RoboCompInnerModelManager::OperationInvalidNode;
		std::ostringstream oss;
		oss <<msg.toStdString() <<" Forbidden, cannot move Node: " <<idSrc.toStdString()<<"to"<<idDst.toStdString()<<"element";
		err.text = oss.str();
		throw err;
	}
	

	InnerModel::NodePtr nodeSrc = getNode(idSrc, msg);
	checkOperationInvalidNode(nodeSrc,msg);
	
	InnerModel::NodePtr nodeDst = getNode(idDst, msg);
	checkOperationInvalidNode(nodeDst,msg);

	QStringList l;
	l.clear();
	
	//consigo ids para viewer
	innerModel->getSubTree (nodeSrc,&l);
	//muevo 
	innerModel->moveSubTree(nodeSrc,nodeDst);
	
	
	/// Replicate InnerModel node removals in the InnerModelViewer tree. And in handlers Lists
	foreach(QString n, l) {
		/// Replicate plane removals
		if(imv->meshHash.contains(n)) {
// 			qDebug()<<"/// Replicate meshHash removals"<<n;			
			while(imv->meshHash[n].osgmeshPaths->getNumParents() > 0)
				( imv->meshHash[n].osgmeshPaths->getParent(0))->removeChild(imv->meshHash[n].osgmeshPaths);			
			while(imv->meshHash[n].osgmeshes->getNumParents() > 0)
				( imv->meshHash[n].osgmeshes->getParent(0))->removeChild(imv->meshHash[n].osgmeshes);
			while(imv->meshHash[n].meshMts->getNumParents() > 0)	
				( imv->meshHash[n].meshMts->getParent(0))->removeChild(imv->meshHash[n].meshMts);			
				
			imv->meshHash.remove(n);
// 			meshColision.remove(n);
		}
		/// Replicate transform removals
		if(imv->mts.contains(n)) {
//			qDebug()<<"/// Replicate transform removals";//<<n<<imv->mts[n]->getNumParents();
 			while(imv->mts[n]->getNumParents() > 0) {
				(imv->mts[n]->getParent(0))->removeChild(imv->mts[n]);
 			}			
 			imv->mts.remove(n);
		}
		/// Replicate plane removals
		if(imv->planeMts.contains(n)) {
//			qDebug()<<"/// Replicate plane removals";
			while(imv->planeMts[n]->getNumParents() > 0) {
				((osg::Group *)(imv->planeMts[n]->getParent(0)))->removeChild(imv->planeMts[n]);
			}
			imv->planeMts.remove(n);
			imv->planesHash.remove(n);
		}
		
	}
	foreach(QString n, l) 
	{
		imv->recursiveConstructor(innerModel->getNode<InnerModelNode>(n), imv->mts[innerModel->getNode<InnerModelNode>(n)->parent->getId()], imv->mts, imv->meshHash); 
	}
	
	
// 	qDebug()<<imv->meshHash.size();
// 	qDebug()<<imv->mts.size();
// 	qDebug()<<imv->planeMts.size();
// 	qDebug()<<imv->planesHash.size();
// 	qDebug()<<innerModel->getIDKeys().size();
// 	innerModel->print();
	qDebug()<<"-- fin move Subtree --";
	return true;
}


void SpecificWorker::imm_getAllNodeInformation(const QString &server, RoboCompInnerModelManager::NodeInformationSequence &nodesInfo)
{
	nodesInfo.clear();
	getRecursiveNodeInformation(nodesInfo, innerModel->getRoot());
}


bool SpecificWorker::imm_collide(const string &a, const string &b)
{
	try
	{
		return innerModel->collide(QString::fromStdString(a), QString::fromStdString(b));
	}
	catch (int err)
	{
		RoboCompInnerModelManager::InnerModelManagerError ex;
		ex.err = RoboCompInnerModelManager::NonExistingNode;
		std::ostringstream oss;
		oss << "InnerModelManager::collide: Error, cannot find node ";
		if (err == 1)
		{
			oss << a;
		}
		else
		{
			oss << b;
		}
		ex.text = oss.str();
		printf("ERROR: %s\n", ex.text.c_str());
		throw ex;
	}
}

