#ifndef BIORBD_UTILS_NODE_H
#define BIORBD_UTILS_NODE_H

#include <memory>
#include "biorbdConfig.h"
#include "Utils/UtilsEnum.h"

namespace biorbd {
namespace utils {
class String;

class BIORBD_API Node
{
public:
    Node();
    Node(
            const biorbd::utils::Node& node);
    Node(
            const biorbd::utils::String &name);
    Node(
            const biorbd::utils::String &name,  // Nom du noeud
            const biorbd::utils::String &parentName);
    virtual ~Node();
    void DeepCopy(const biorbd::utils::Node& other);

    const biorbd::utils::String& name() const;
    void setName(const biorbd::utils::String &name);

    const biorbd::utils::String &parent() const;
    void setParent(const biorbd::utils::String &parentName);

    biorbd::utils::NODE_TYPE typeOfNode() const;

protected:
    virtual void setType() = 0;
    std::shared_ptr<biorbd::utils::String> m_name;
    std::shared_ptr<biorbd::utils::String> m_parentName;
    std::shared_ptr<biorbd::utils::NODE_TYPE> m_typeOfNode;

};

}}

#endif // BIORBD_UTILS_NODE_H
