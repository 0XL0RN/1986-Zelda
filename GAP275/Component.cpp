module Component;

Component::Component(Entity* pEntity)
	: m_pEntity(pEntity)
{
}

Entity* Component::GetEntity() const
{
	return m_pEntity;
}

void Component::SetEntity(Entity* pEntity)
{
	m_pEntity = pEntity;
}
