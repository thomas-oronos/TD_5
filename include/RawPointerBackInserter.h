/// Itérateur inséreur arrière.
/// \author Misha Krieger-Raynauld
/// \date 2020-01-12

#ifndef RAWPOINTERBACKINSERTER_H
#define RAWPOINTERBACKINSERTER_H

#include <memory>

/// Itérateur inséreur arrière servant à insérer un pointeur brut dans un conteneur à partir
/// de valeurs de std::unique_ptr (semblable à std::back_insert_iterator).
/// \tparam Container   Le type du conteneur de destination contenant des pointeurs bruts.
template<typename Container>
class RawPointerBackInserter
{
public:
    template<typename Deleter>
    using uniquePointerType =
        std::unique_ptr<std::remove_const_t<std::remove_pointer_t<typename Container::value_type>>, Deleter>;

    /// Constructeur qui initialise le pointeur interne vers le conteneur à partir du conteneur passé en paramètre.
    /// \param conteneur    Le conteneur dans lequel insérer les éléments lorsque l'itérateur est assigné.
    explicit RawPointerBackInserter(Container& conteneur)
        : conteneur_(&conteneur)
    {
    }

    /// Insère le pointer brut conservé par le std::unique_ptr dans le conteneur en appelant sa fonction push_back.
    /// \param value    Le std::unique_ptr possédant la valeur à insérer à l'arrière du conteneur.
    /// \return         Une référence à l'itérateur.
    template<typename Deleter>
    RawPointerBackInserter<Container>& operator=(const uniquePointerType<Deleter>& value)
    {
        conteneur_->push_back(value.get());
        return *this;
    }

    /// Surcharge supprimée pour empêcher des pointeurs bruts vers de la mémoire désallouée.
    /// \return Une référence à l'itérateur.
    template<typename Deleter>
    RawPointerBackInserter<Container>& operator=(uniquePointerType<Deleter>&&) = delete;

    /// No-op.
    /// \return Une référence à l'itérateur.
    RawPointerBackInserter<Container>& operator*() { return *this; }

    /// No-op.
    /// \return Une référence à l'itérateur.
    RawPointerBackInserter<Container>& operator++() { return *this; }

    /// No-op.
    /// \return Une référence à l'itérateur.
    RawPointerBackInserter<Container>& operator++(int) { return *this; }

private:
    Container* conteneur_;
};

#endif // RAWPOINTERBACKINSERTER_H
