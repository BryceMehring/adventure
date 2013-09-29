#ifndef _IDESTROYABLE_
#define _IDESTROYABLE_


// Destroyable interface, Any object that must be destroyed, should inherit from this class.
class IDestroyable
{
public:

	virtual ~IDestroyable() {}

	virtual void Destroy() = 0;

	static const unsigned int INTERFACE_DESTROY = 924490;
};

#endif // _IDESTROYABLE_
