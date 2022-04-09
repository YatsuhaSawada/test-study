#pragma

#define TL_EXPECTED_VERSION_MAJOR 1
#define TL_EXPECTED_VERSION_MINOR 0
#define TL_EXPECTED_VERSION_PATCH 1

#include <exception>
#include <functional>
#include <type_traits>
#include <utility>

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define TL_EXPECTED_EXCEPTIONS_ENABLED
#endif

#if defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define TL_EXPECTED_EXCEPTIONS_ENABLED
#endif

#if (defined(_MSC_VER) && _MSC_VER == 1900)
#define TL_EXPECTED_MSVC2015
#define TL_EXPECTED_MSVC2015_CONSTEXPR
#else
#define TL_EXPECTED_MSVC2015_CONSTEXPR constexpr
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC49
#endif

#if (defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ <= 4 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC54
#endif

#if (defined(__GNUC__) && __GNUC__ == 5 && __GNUC_MINOR__ <= 5 &&              \
     !defined(__clang__))
#define TL_EXPECTED_GCC55
#endif

#if (defined(__GNUC__) && __GNUC__ == 4 && __GNUC_MINOR__ <= 9 &&              \
     !defined(__clang__))
// GCC < 5 doesn't support overloading on const&& for member functions

#define TL_EXPECTED_NO_CONSTRR
// GCC < 5 doesn't support some standard C++11 type traits
#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                         \
  std::has_trivial_copy_constructor<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                            \
  std::has_trivial_copy_assign<T>

// This one will be different for GCC 5.7 if it's ever supported
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)                               \
  std::is_trivially_destructible<T>

// GCC 5 < v < 8 has a bug in is_trivially_copy_constructible which breaks std::vector
// for non-copyable types
#elif (defined(__GNUC__) && __GNUC__ < 8 &&                                                \
     !defined(__clang__))
#ifndef TL_GCC_LESS_8_TRIVIALLY_COPY_CONSTRUCTIBLE_MUTEX
#define TL_GCC_LESS_8_TRIVIALLY_COPY_CONSTRUCTIBLE_MUTEX
namespace tl {
    namespace detail {
        template<class T>
        struct is_trivially_copy_constructible : std::is_trivially_copy_constructible<T> {};
#ifdef _GLIBCXX_VECTOR
        template<class T, class A>
        struct is_trivially_copy_constructible<std::vector<T, A>>
            : std::false_type {};
#endif
    }
}
#endif

#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                                     \
  tl::detail::is_trivially_copy_constructible<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                                        \
  std::is_trivially_copy_assignable<T>
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T) std::is_trivially_destructible<T>
#else
#define TL_EXPECTED_IS_TRIVIALLY_COPY_CONSTRUCTIBLE(T)                         \
  std::is_trivially_copy_constructible<T>
#define TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T)                            \
  std::is_trivially_copy_assignable<T>
#define TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)                               \
  std::is_trivially_destructible<T>
#endif

#if __cplusplus > 201103L
#define TL_EXPECTED_CXX14
#endif

#ifdef TL_EXPECTED_GCC49
#define TL_EXPECTED_GCC49_CONSTEXPR
#else
#define TL_EXPECTED_GCC49_CONSTEXPR constexpr
#endif

#if (__cplusplus == 201103L || defined(TL_EXPECTED_MSVC2015) ||                \
     defined(TL_EXPECTED_GCC49))
#define TL_EXPECTED_11_CONSTEXPR
#else
#define TL_EXPECTED_11_CONSTEXPR constexpr
#endif

namespace ren {
    template <class T, class E> class expected;

    class monostate {};

    // C++17����T�|�[�g�����in_place_t �� ���O�Œ�`
    struct in_place_t {
        explicit in_place_t() = default;
    };
    static constexpr in_place_t in_place{};

    // expected<T, E>��T==E�̏ꍇ�ɓ��������������\�b�h���쐬����邱�Ƃ�h�����߂̃N���X
    template <class E> class unexpected {
    public:
        static_assert(!std::is_same<E, void>::value, "E must not be void");

        unexpected() = delete;
        constexpr explicit unexpected(const E& e) : m_val(e) {}
        constexpr explicit unexpected(E&& e) : m_val(std::move(e)) {}

        constexpr const E& value() const& { return m_val; }
        constexpr E& value()& { return m_val; }
        constexpr E&& value()&& { return std::move(m_val); }
        constexpr const E&& value() const&& { return std::move(m_val); }

    private:
        E m_val;
    };

    template <class E>
    constexpr bool operator==(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() == rhs.value();
    }
    template <class E>
    constexpr bool operator!=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() != rhs.value();
    }
    template <class E>
    constexpr bool operator<(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() < rhs.value();
    }
    template <class E>
    constexpr bool operator<=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() <= rhs.value();
    }
    template <class E>
    constexpr bool operator>(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() > rhs.value();
    }
    template <class E>
    constexpr bool operator>=(const unexpected<E>& lhs, const unexpected<E>& rhs) {
        return lhs.value() >= rhs.value();
    }

    // unexpected<E>�𐶐�����̂�������֐�
    template <class E>
    unexpected<typename std::decay<E>::type> make_unexpected(E&& e) {
        return unexpected<typename std::decay<E>::type>(std::forward<E>(e));
    }

    struct unexpect_t {
        unexpect_t() = default;
    };
    static constexpr unexpect_t unexpect{};

    namespace detail {
        template<typename E>
        [[noreturn]] constexpr void throw_exception(E&& e) {
#ifdef TL_EXPECTED_EXCEPTIONS_ENABLED
            throw std::forward<E>(e);
#else
            __builtin_unreachable();
#endif
        }

        // ==================================================================
        // C++14-style aliases for brevity
        template <class T> using remove_const_t = typename std::remove_const<T>::type;
        template <class T>
        using remove_reference_t = typename std::remove_reference<T>::type;
        template <class T> using decay_t = typename std::decay<T>::type;
        template <bool E, class T = void>
        using enable_if_t = typename std::enable_if<E, T>::type;
        template <bool B, class T, class F>
        using conditional_t = typename std::conditional<B, T, F>::type;

        // std::conjunction from C++17
        template <class...> struct conjunction : std::true_type {};
        template <class B> struct conjunction<B> : B {};
        template <class B, class... Bs>
        struct conjunction<B, Bs...>
            : std::conditional<bool(B::value), conjunction<Bs...>, B>::type {};

        // std::invoke from C++17
        // https://stackoverflow.com/questions/38288042/c11-14-invoke-workaround
        template <typename Fn, typename... Args,
#ifdef TL_TRAITS_LIBCXX_MEM_FN_WORKAROUND
            typename = enable_if_t<!(is_pointer_to_non_const_member_func<Fn>::value
                && is_const_or_const_ref<Args...>::value)>,
#endif
            typename = enable_if_t<std::is_member_pointer<decay_t<Fn>>::value>,
            int = 0>
            constexpr auto invoke(Fn&& f, Args && ... args) noexcept(
                noexcept(std::mem_fn(f)(std::forward<Args>(args)...)))
            -> decltype(std::mem_fn(f)(std::forward<Args>(args)...)) {
            return std::mem_fn(f)(std::forward<Args>(args)...);
        }

        template <typename Fn, typename... Args,
            typename = enable_if_t<!std::is_member_pointer<decay_t<Fn>>::value>>
            constexpr auto invoke(Fn&& f, Args && ... args) noexcept(
                noexcept(std::forward<Fn>(f)(std::forward<Args>(args)...)))
            -> decltype(std::forward<Fn>(f)(std::forward<Args>(args)...)) {
            return std::forward<Fn>(f)(std::forward<Args>(args)...);
        }

        // std::invoke_result from C++17
        template <class F, class, class... Us> struct invoke_result_impl;

        template <class F, class... Us>
        struct invoke_result_impl<
            F, decltype(detail::invoke(std::declval<F>(), std::declval<Us>()...), void()),
            Us...> {
            using type = decltype(detail::invoke(std::declval<F>(), std::declval<Us>()...));
        };

        template <class F, class... Us>
        using invoke_result = invoke_result_impl<F, void, Us...>;

        template <class F, class... Us>
        using invoke_result_t = typename invoke_result<F, Us...>::type;

        // https://stackoverflow.com/questions/26744589/what-is-a-proper-way-to-implement-is-swappable-to-test-for-the-swappable-concept
        namespace swap_adl_tests {
            // if swap ADL finds this then it would call std::swap otherwise (same
            // signature)
            struct tag {};

            template <class T> tag swap(T&, T&);
            template <class T, std::size_t N> tag swap(T(&a)[N], T(&b)[N]);

            // helper functions to test if an unqualified swap is possible, and if it
            // becomes std::swap
            template <class, class> std::false_type can_swap(...) noexcept(false);
            template <class T, class U,
                class = decltype(swap(std::declval<T&>(), std::declval<U&>()))>
                std::true_type can_swap(int) noexcept(noexcept(swap(std::declval<T&>(),
                    std::declval<U&>())));

            template <class, class> std::false_type uses_std(...);
            template <class T, class U>
            std::is_same<decltype(swap(std::declval<T&>(), std::declval<U&>())), tag>
                uses_std(int);

            template <class T>
            struct is_std_swap_noexcept
                : std::integral_constant<bool,
                std::is_nothrow_move_constructible<T>::value&&
                std::is_nothrow_move_assignable<T>::value> {};

            template <class T, std::size_t N>
            struct is_std_swap_noexcept<T[N]> : is_std_swap_noexcept<T> {};

            template <class T, class U>
            struct is_adl_swap_noexcept
                : std::integral_constant<bool, noexcept(can_swap<T, U>(0))> {};
        } // namespace swap_adl_tests

        template <class T, class U = T>
        struct is_swappable
            : std::integral_constant<
            bool,
            decltype(detail::swap_adl_tests::can_swap<T, U>(0))::value &&
            (!decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value ||
                (std::is_move_assignable<T>::value &&
                    std::is_move_constructible<T>::value))> {};

        template <class T, std::size_t N>
        struct is_swappable<T[N], T[N]>
            : std::integral_constant<
            bool,
            decltype(detail::swap_adl_tests::can_swap<T[N], T[N]>(0))::value &&
            (!decltype(
                detail::swap_adl_tests::uses_std<T[N], T[N]>(0))::value ||
                is_swappable<T, T>::value)> {};

        template <class T, class U = T>
        struct is_nothrow_swappable
            : std::integral_constant<
            bool,
            is_swappable<T, U>::value &&
            ((decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value
                && detail::swap_adl_tests::is_std_swap_noexcept<T>::value) ||
                (!decltype(detail::swap_adl_tests::uses_std<T, U>(0))::value &&
                    detail::swap_adl_tests::is_adl_swap_noexcept<T,
                    U>::value))> {
        };

        // ==================================================================
        // Trait for checking if a type is a expected
        template <class T> struct is_expected_impl : std::false_type {};
        template <class T, class E>
        struct is_expected_impl<expected<T, E>> : std::true_type {};
        template <class T> using is_expected = is_expected_impl<decay_t<T>>;

        template <class T, class E, class U>
        using expected_enable_forward_value = detail::enable_if_t<
            std::is_constructible<T, U&&>::value &&
            !std::is_same<detail::decay_t<U>, in_place_t>::value &&
            !std::is_same<expected<T, E>, detail::decay_t<U>>::value &&
            !std::is_same<unexpected<E>, detail::decay_t<U>>::value>;

        template <class T, class E, class U, class G, class UR, class GR>
        using expected_enable_from_other = detail::enable_if_t<
            std::is_constructible<T, UR>::value&&
            std::is_constructible<E, GR>::value &&
            !std::is_constructible<T, expected<U, G>&>::value &&
            !std::is_constructible<T, expected<U, G>&&>::value &&
            !std::is_constructible<T, const expected<U, G>&>::value &&
            !std::is_constructible<T, const expected<U, G>&&>::value &&
            !std::is_convertible<expected<U, G>&, T>::value &&
            !std::is_convertible<expected<U, G>&&, T>::value &&
            !std::is_convertible<const expected<U, G>&, T>::value &&
            !std::is_convertible<const expected<U, G>&&, T>::value>;

        template <class T, class U>
        using is_void_or = conditional_t<std::is_void<T>::value, std::true_type, U>;

        template <class T>
        using is_copy_constructible_or_void =
            is_void_or<T, std::is_copy_constructible<T>>;

        template <class T>
        using is_move_constructible_or_void =
            is_void_or<T, std::is_move_constructible<T>>;

        template <class T>
        using is_copy_assignable_or_void =
            is_void_or<T, std::is_copy_assignable<T>>;


        template <class T>
        using is_move_assignable_or_void =
            is_void_or<T, std::is_move_assignable<T>>;
    } // namespace detail


    namespace detail {
        struct no_init_t {};
        static constexpr no_init_t no_init{};

        // expected�̒l�̕ۑ�����������N���X�ł��B�������̓��ꉻ������A
        // ���̓��ꉻ�́A `T` �� `E` �̂ǂ���������ɂ͔j��ł��Ȃ��ꍇ�̂��߂ɂ���܂��B
        // ���������āA�f�X�g���N�^�� `expected` �̔j�󎞂ɌĂяo����Ȃ���΂Ȃ�܂���B
        template <
            class T,
            class E,
            bool = std::is_trivially_destructible<T>::value, //T�̃f�X�g���N�^�̓g���r�A�����H
            bool = std::is_trivially_destructible<E>::value  //E�̃f�X�g���N�^�̓g���r�A�����H
        >
        struct expected_storage_base {
            constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
            constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

            // T( Args... )�̌`���̃R���X�g���N�^�Ăяo�����K�i�ł���Ȃ��`�����R���X�g���N�^
            template <class... Args,
                detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* = nullptr>
            constexpr expected_storage_base(in_place_t, Args &&... args)
                : m_val(std::forward<Args>(args)...), m_has_val(true) {}

            // T�ɑ΂���{Args...}�̌`���̃R���X�g���N�^�Ăяo�����K�i�ł���Ȃ��`�����R���X�g���N�^
            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                    T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
                    Args &&... args)
                : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}

            // E( Args... )�̌`���̃R���X�g���N�^�Ăяo�����K�i�ł���Ȃ��`�����R���X�g���N�^
            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
            constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            // E�ɑ΂���{Args...}�̌`���̃R���X�g���N�^�Ăяo�����K�i�ł���Ȃ��`�����R���X�g���N�^
            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr explicit expected_storage_base(
                    unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() {
                if (m_has_val) {
                    m_val.~T();
                }
                else {
                    m_unexpect.~unexpected<E>();
                }
            }
            union {
                T m_val;
                unexpected<E> m_unexpect;
                char m_no_init;
            };
            bool m_has_val;
        };

        // ���̓��ꉻ�́A `T` �� `E` �̗����������I�ɔj��\�ł���ꍇ�̂��߂̂��̂ł���B
        // ���̂��߁A`expected` �̃f�X�g���N�^�̓g���r�A���Ȃ��̂ł悢�̂ł��B
        template <class T, class E> struct expected_storage_base<T, E, true, true> {
            constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
            constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

            template <class... Args,
                detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
                nullptr>
            constexpr expected_storage_base(in_place_t, Args &&... args)
                : m_val(std::forward<Args>(args)...), m_has_val(true) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
                    Args &&... args)
                : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
            constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr explicit expected_storage_base(unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() = default;
            union {
                T m_val;
                unexpected<E> m_unexpect;
                char m_no_init;
            };
            bool m_has_val;
        };

        // T is trivial, E is not.
        template <class T, class E> struct expected_storage_base<T, E, true, false> {
            constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
            constexpr expected_storage_base(no_init_t)
                : m_no_init(), m_has_val(false) {}

            template <class... Args,
                detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
                nullptr>
                constexpr expected_storage_base(in_place_t, Args &&... args)
                : m_val(std::forward<Args>(args)...), m_has_val(true) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
                    Args &&... args)
                : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
                constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr explicit expected_storage_base(unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() {
                if (!m_has_val) {
                    m_unexpect.~unexpected<E>();
                }
            }

            union {
                T m_val;
                unexpected<E> m_unexpect;
                char m_no_init;
            };
            bool m_has_val;
        };

        // E is trivial, T is not.
        template <class T, class E> struct expected_storage_base<T, E, false, true> {
            constexpr expected_storage_base() : m_val(T{}), m_has_val(true) {}
            constexpr expected_storage_base(no_init_t) : m_no_init(), m_has_val(false) {}

            template <class... Args,
                detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
                nullptr>
                constexpr expected_storage_base(in_place_t, Args &&... args)
                : m_val(std::forward<Args>(args)...), m_has_val(true) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr expected_storage_base(in_place_t, std::initializer_list<U> il,
                    Args &&... args)
                : m_val(il, std::forward<Args>(args)...), m_has_val(true) {}
            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
                constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr explicit expected_storage_base(unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() {
                if (m_has_val) {
                    m_val.~T();
                }
            }
            union {
                T m_val;
                unexpected<E> m_unexpect;
                char m_no_init;
            };
            bool m_has_val;
        };

        // `T` is `void`, `E` is trivially-destructible
        template <class E> struct expected_storage_base<void, E, false, true> {
            constexpr expected_storage_base() : m_has_val(true) {}
            constexpr expected_storage_base(no_init_t) : m_val(), m_has_val(false) {}

            constexpr expected_storage_base(in_place_t) : m_has_val(true) {}

            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
                constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr explicit expected_storage_base(unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() = default;
            struct dummy {};
            union {
                unexpected<E> m_unexpect;
                dummy m_val;
            };
            bool m_has_val;
        };

        // `T` is `void`, `E` is not trivially-destructible
        template <class E> struct expected_storage_base<void, E, false, false> {
            constexpr expected_storage_base() : m_dummy(), m_has_val(true) {}
            constexpr expected_storage_base(no_init_t) : m_dummy(), m_has_val(false) {}

            constexpr expected_storage_base(in_place_t) : m_dummy(), m_has_val(true) {}

            template <class... Args,
                detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
                nullptr>
                constexpr explicit expected_storage_base(unexpect_t, Args &&... args)
                : m_unexpect(std::forward<Args>(args)...), m_has_val(false) {}

            template <class U, class... Args,
                detail::enable_if_t<std::is_constructible<
                E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
                constexpr explicit expected_storage_base(unexpect_t,
                    std::initializer_list<U> il,
                    Args &&... args)
                : m_unexpect(il, std::forward<Args>(args)...), m_has_val(false) {}

            ~expected_storage_base() {
                if (!m_has_val) {
                    m_unexpect.~unexpected<E>();
                }
            }

            union {
                unexpected<E> m_unexpect;
                char m_dummy;
            };
            bool m_has_val;
        };

        // ���̊��N���X�́A����ɔh���N���X�Ŏg�p�ł��邢�����֗̕��ȃ����o�֐���񋟂��܂��B
        template <class T, class E>
        struct expected_operations_base : expected_storage_base<T, E> {
            using expected_storage_base<T, E>::expected_storage_base;

            template <class... Args> void construct(Args &&... args) noexcept {
                new (std::addressof(this->m_val)) T(std::forward<Args>(args)...);
                this->m_has_val = true;
            }

            template <class Rhs> void construct_with(Rhs&& rhs) noexcept {
                new (std::addressof(this->m_val)) T(std::forward<Rhs>(rhs).get());
                this->m_has_val = true;
            }

            template <class... Args> void construct_error(Args &&... args) noexcept {
                new (std::addressof(this->m_unexpect))
                    unexpected<E>(std::forward<Args>(args)...);
                this->m_has_val = false;
            }

            // �����̑���I�[�o�[���[�h�́A���͂ȗ�O�ۏ؂��ێ����Ȃ���A�ł������I��
            // ����������g�p����邱�Ƃ�ۏ؂��܂��B
            // ���ƂȂ�̂́Arhs���l�������A* this���l�������Ȃ��ꍇ�ł���B
            // ���̃I�[�o�[���[�h�́A�P�ɃR�s�[�\���� `T` ���X���[���邱�ƂȂ��A���ڂ��̏��
            // �u�����Ƃ��ł��܂��B
            template <class U = T,
                detail::enable_if_t<std::is_nothrow_copy_constructible<U>::value>* = nullptr>
                void assign(const expected_operations_base& rhs) noexcept {
                if (!this->m_has_val && rhs.m_has_val) {
                    geterr().~unexpected<E>();
                    construct(rhs.get());
                }
                else {
                    assign_common(rhs);
                }
            }

            // ���̃I�[�o�[���[�h�́A`T`�̃R�s�[���쐬���A�R�s�[�����������ꍇ�ɂ����
            // ����̈ʒu�Ƀm�[�X���[�ňړ������悤�Ƃ���P�[�X����������B
            template <class U = T,
                detail::enable_if_t<!std::is_nothrow_copy_constructible<U>::value&&
                std::is_nothrow_move_constructible<U>::value>
                * = nullptr>
                void assign(const expected_operations_base& rhs) noexcept {
                if (!this->m_has_val && rhs.m_has_val) {
                    T tmp = rhs.get();
                    geterr().~unexpected<E>();
                    construct(std::move(tmp));
                }
                else {
                    assign_common(rhs);
                }
            }

            // ���̃I�[�o�[���[�h�͍ň��̃P�[�X�ł���A���̏ꍇ�A"move-construct "
            // ���Ȃ���΂Ȃ�܂���B
            // �\�����Ȃ��l���ꎞ�ۑ����AT������̈ʒu�ɃR�s�[���悤�Ƃ��܂��B
            // �����A�\�z����������΁A���ׂĂ����܂��������A�����A�X���[�����ꍇ�B
            // �̏ꍇ�A�Â��\�����ʒl�����̏ꏊ�ɖ߂��Ă����O���ăX���[���܂��B
            template <class U = T,
                detail::enable_if_t<!std::is_nothrow_copy_constructible<U>::value &&
                !std::is_nothrow_move_constructible<U>::value>
                * = nullptr>
                void assign(const expected_operations_base& rhs) {
                if (!this->m_has_val && rhs.m_has_val) {
                    auto tmp = std::move(geterr());
                    geterr().~unexpected<E>();

#ifdef EXPECTED_EXCEPTIONS_ENABLED
                    try {
                        construct(rhs.get());
                    }
                    catch (...) {
                        geterr() = std::move(tmp);
                        throw;
                    }
#else
                    construct(rhs.get());
#endif
                }
                else {
                    assign_common(rhs);
                }
            }

            //  �����̃I�[�o�[���[�h�́Arvalues�ɂ��Ă͏�L�Ɠ������Ƃ��s��
            template <class U = T,
                detail::enable_if_t<std::is_nothrow_move_constructible<U>::value>
                * = nullptr>
                void assign(expected_operations_base&& rhs) noexcept {
                if (!this->m_has_val && rhs.m_has_val) {
                    geterr().~unexpected<E>();
                    construct(std::move(rhs).get());
                }
                else {
                    assign_common(std::move(rhs));
                }
            }

            template <class U = T,
                detail::enable_if_t<!std::is_nothrow_move_constructible<U>::value>
                * = nullptr>
                void assign(expected_operations_base&& rhs) {
                if (!this->m_has_val && rhs.m_has_val) {
                    auto tmp = std::move(geterr());
                    geterr().~unexpected<E>();
#ifdef EXPECTED_EXCEPTIONS_ENABLED
                    try {
                        construct(std::move(rhs).get());
                    }
                    catch (...) {
                        geterr() = std::move(tmp);
                        throw;
                    }
#else
                    construct(std::move(rhs).get());
#endif
                }
                else {
                    assign_common(std::move(rhs));
                }
            }

            // ���[�u�E�R�s�[�E�A�T�C�����ʕ�
            template <class Rhs> void assign_common(Rhs&& rhs) {
                if (this->m_has_val) {
                    if (rhs.m_has_val) {
                        get() = std::forward<Rhs>(rhs).get();
                    }
                    else {
                        destroy_val();
                        construct_error(std::forward<Rhs>(rhs).geterr());
                    }
                }
                else {
                    if (!rhs.m_has_val) {
                        geterr() = std::forward<Rhs>(rhs).geterr();
                    }
                }
            }

            bool has_value() const { return this->m_has_val; }

            constexpr T& get()& { return this->m_val; }
            constexpr const T& get() const& { return this->m_val; }
            constexpr T&& get()&& { return std::move(this->m_val); }

            constexpr const T&& get() const&& { return std::move(this->m_val); }

            constexpr unexpected<E>& geterr()& {
                return this->m_unexpect;
            }
            constexpr const unexpected<E>& geterr() const& { return this->m_unexpect; }
            constexpr unexpected<E>&& geterr()&& {
                return std::move(this->m_unexpect);
            }
            constexpr const unexpected<E>&& geterr() const&& {
                return std::move(this->m_unexpect);
            }
            constexpr void destroy_val() {
                get().~T();
            }
        };

        // ���̊��N���X�́A����ɔh���N���X�Ŏg�p�ł��邢�����֗̕��ȃ����o�֐���񋟂��܂��B
        template <class E>
        struct expected_operations_base<void, E> : expected_storage_base<void, E> {
            using expected_storage_base<void, E>::expected_storage_base;

            template <class... Args> void construct() noexcept { this->m_has_val = true; }

            // ���̊֐��͈������g�p���܂��񂪁A������K�v�Ƃ��܂��B
            // �������̃��x���ł́AT �� void �ł��邩�ǂ����Ɋ֌W�Ȃ����삵�܂��B
            template <class Rhs>
            void construct_with(Rhs&&) noexcept {
                this->m_has_val = true;
            }

            template <class... Args>
            void construct_error(Args &&... args) noexcept {
                new (std::addressof(this->m_unexpect))
                    unexpected<E>(std::forward<Args>(args)...);
                this->m_has_val = false;
            }

            template <class Rhs>
            void assign(Rhs&& rhs) noexcept {
                if (!this->m_has_val) {
                    if (rhs.m_has_val) {
                        geterr().~unexpected<E>();
                        construct();
                    }
                    else {
                        geterr() = std::forward<Rhs>(rhs).geterr();
                    }
                }
                else {
                    if (!rhs.m_has_val) {
                        construct_error(std::forward<Rhs>(rhs).geterr());
                    }
                }
            }

            bool has_value() const { return this->m_has_val; }

            constexpr unexpected<E>& geterr()& {
                return this->m_unexpect;
            }
            constexpr const unexpected<E>& geterr() const& { return this->m_unexpect; }
            constexpr unexpected<E>&& geterr()&& {
                return std::move(this->m_unexpect);
            }

            constexpr const unexpected<E>&& geterr() const&& {
                return std::move(this->m_unexpect);
            }

            constexpr void destroy_val() {
                //no-op
            }
        };

        // ���̃N���X�́A�����t���ō��ׂȃR�s�[�R���X�g���N�^�������Ƃ��Ǘ����܂��B
        // ���̓��ꉻ�́AT �� E ��trivial�ɃR�s�[�\���\�ł���ꍇ�̂��߂̂��̂ł���
        template <class T, class E,
            bool = is_void_or<T, std::is_trivially_copy_constructible<T>>::value
            && std::is_trivially_copy_constructible<E>::value
        >
        struct expected_copy_base : expected_operations_base<T, E> {
            using expected_operations_base<T, E>::expected_operations_base;
        };

        // ���̓��ꉻ�́AT�܂���E��trivial�ȃR�s�[�\���\�łȂ��Ƃ��̂��߂ɂ���
        template <class T, class E>
        struct expected_copy_base<T, E, false> : expected_operations_base<T, E> {
            using expected_operations_base<T, E>::expected_operations_base;

            expected_copy_base() = default;
            expected_copy_base(const expected_copy_base& rhs)
                : expected_operations_base<T, E>(no_init) {
                if (rhs.has_value()) {
                    this->construct_with(rhs);
                }
                else {
                    this->construct_error(rhs.geterr());
                }
            }

            expected_copy_base(expected_copy_base&& rhs) = default;
            expected_copy_base& operator=(const expected_copy_base& rhs) = default;
            expected_copy_base& operator=(expected_copy_base&& rhs) = default;
        };

        // ���̃N���X�͏����t���ł܂�Ȃ��ړ��R���X�g���N�^�������Ƃ��Ǘ����܂��B
        // �c�O�Ȃ���AGCC < 5 AFAIK �ł� std::is_trivially_move_constructible �̃A�i���O��
        // �������Ȃ��̂ŁA�����B��������@�͂���܂���BT�������Ȉړ��\���\�ł����Ă��A
        // �����łȂ��ړ��R���X�g���N�^�ł��߂�����������܂���B
        template <class T, class E,
            bool = is_void_or<T, std::is_trivially_move_constructible<T>>::value
            && std::is_trivially_move_constructible<E>::value>
            struct expected_move_base : expected_copy_base<T, E> {
            using expected_copy_base<T, E>::expected_copy_base;
        };

        template <class T, class E>
        struct expected_move_base<T, E, false> : expected_copy_base<T, E> {
            using expected_copy_base<T, E>::expected_copy_base;

            expected_move_base() = default;
            expected_move_base(const expected_move_base& rhs) = default;

            expected_move_base(expected_move_base&& rhs) noexcept(
                std::is_nothrow_move_constructible<T>::value)
                : expected_copy_base<T, E>(no_init) {
                if (rhs.has_value()) {
                    this->construct_with(std::move(rhs));
                }
                else {
                    this->construct_error(std::move(rhs.geterr()));
                }
            }
            expected_move_base& operator=(const expected_move_base& rhs) = default;
            expected_move_base& operator=(expected_move_base&& rhs) = default;
        };

        // ���̃N���X�́Ctrivial�ȃR�s�[������Z�q�������Ƃ������Ƃ��ĊǗ�����B
        template <class T, class E,
            bool = is_void_or<
            T, conjunction<TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(T),
            std::is_trivially_copy_constructible<T>,
            TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(T)>>::value
            && TL_EXPECTED_IS_TRIVIALLY_COPY_ASSIGNABLE(E)::value
            && std::is_trivially_copy_constructible<E>::value
            && TL_EXPECTED_IS_TRIVIALLY_DESTRUCTIBLE(E)::value>
            struct expected_copy_assign_base : expected_move_base<T, E> {
            using expected_move_base<T, E>::expected_move_base;
        };

        template <class T, class E>
        struct expected_copy_assign_base<T, E, false> : expected_move_base<T, E> {
            using expected_move_base<T, E>::expected_move_base;

            expected_copy_assign_base() = default;
            expected_copy_assign_base(const expected_copy_assign_base& rhs) = default;

            expected_copy_assign_base(expected_copy_assign_base&& rhs) = default;
            expected_copy_assign_base& operator=(const expected_copy_assign_base& rhs) {
                this->assign(rhs);
                return *this;
            }
            expected_copy_assign_base&
                operator=(expected_copy_assign_base&& rhs) = default;
        };

        // ���̃N���X�́A�����t���ŁA�g���r�A���Ȉړ��̊��蓖�ĉ��Z�q�������Ƃ��Ǘ����܂��B 
        // �c�O�Ȃ���AGCC < 5 AFAIK�ł́Astd::is_trivially_move_assignable�̃A�i���O���������Ȃ��̂ŁA
        // �����B��������@�͂���܂���BT�������Ȉړ����蓖�ĉ\�ł����Ă��A�����łȂ��ړ����蓖��
        // ���Z�q�ł�肷���������Ȃ��̂ł��B
        template <
            class T,
            class E,
            bool =
            is_void_or<T,
            conjunction<
            std::is_trivially_destructible<T>,
            std::is_trivially_move_constructible<T>,
            std::is_trivially_move_assignable<T>>>::value
            && std::is_trivially_destructible<E>::value
            && std::is_trivially_move_constructible<E>::value
            && std::is_trivially_move_assignable<E>::value
            >
            struct expected_move_assign_base : expected_copy_assign_base<T, E> {
            using expected_copy_assign_base<T, E>::expected_copy_assign_base;
        };

        template <class T, class E>
        struct expected_move_assign_base<T, E, false>
            : expected_copy_assign_base<T, E> {
            using expected_copy_assign_base<T, E>::expected_copy_assign_base;

            expected_move_assign_base() = default;
            expected_move_assign_base(const expected_move_assign_base& rhs) = default;

            expected_move_assign_base(expected_move_assign_base&& rhs) = default;

            expected_move_assign_base&
                operator=(const expected_move_assign_base& rhs) = default;

            expected_move_assign_base&
                operator=(expected_move_assign_base&& rhs) noexcept(
                    std::is_nothrow_move_constructible<T>::value
                    && std::is_nothrow_move_assignable<T>::value) {
                this->assign(std::move(rhs));
                return *this;
            }
        };

        // expected_delete_ctor_base will conditionally delete copy and move
        // constructors depending on whether T is copy/move constructible
        // expected_delete_ctor_base �́AT ���R�s�[/���[�u�R���X�g���N�^�u��
        // ���ǂ����ɉ����āA�R�s�[�ƃ��[�u�R���X�g���N�^�������t���ō폜���܂��B
        template <class T, class E,
            bool EnableCopy = (is_copy_constructible_or_void<T>::value&&
                std::is_copy_constructible<E>::value),
            bool EnableMove = (is_move_constructible_or_void<T>::value&&
                std::is_move_constructible<E>::value)>
            struct expected_delete_ctor_base {
            expected_delete_ctor_base() = default;
            expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
            expected_delete_ctor_base&
                operator=(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base&
                operator=(expected_delete_ctor_base&&) noexcept = default;
        };

        template <class T, class E>
        struct expected_delete_ctor_base<T, E, true, false> {
            expected_delete_ctor_base() = default;
            expected_delete_ctor_base(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
            expected_delete_ctor_base&
                operator=(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base&
                operator=(expected_delete_ctor_base&&) noexcept = default;
        };

        template <class T, class E>
        struct expected_delete_ctor_base<T, E, false, true> {
            expected_delete_ctor_base() = default;
            expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
            expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = default;
            expected_delete_ctor_base&
                operator=(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base&
                operator=(expected_delete_ctor_base&&) noexcept = default;
        };

        template <class T, class E>
        struct expected_delete_ctor_base<T, E, false, false> {
            expected_delete_ctor_base() = default;
            expected_delete_ctor_base(const expected_delete_ctor_base&) = delete;
            expected_delete_ctor_base(expected_delete_ctor_base&&) noexcept = delete;
            expected_delete_ctor_base&
                operator=(const expected_delete_ctor_base&) = default;
            expected_delete_ctor_base&
                operator=(expected_delete_ctor_base&&) noexcept = default;
        };

        // expected_delete_assign_base will conditionally delete copy and move
        // constructors depending on whether T and E are copy/move constructible +
        // assignable
        // expected_delete_assign_base �́AT �� E ��
        // �R�s�[/���[�u�R���X�g���N�^�u�� + �A�T�C���\���ǂ����ɉ����āA
        // �����t���ŃR�s�[/���[�u�R���X�g���N�^���폜���܂��B
        template <class T, class E,
            bool EnableCopy = (is_copy_constructible_or_void<T>::value&&
                std::is_copy_constructible<E>::value&&
                is_copy_assignable_or_void<T>::value&&
                std::is_copy_assignable<E>::value),
            bool EnableMove = (is_move_constructible_or_void<T>::value&&
                std::is_move_constructible<E>::value&&
                is_move_assignable_or_void<T>::value&&
                std::is_move_assignable<E>::value)>
            struct expected_delete_assign_base {
            expected_delete_assign_base() = default;
            expected_delete_assign_base(const expected_delete_assign_base&) = default;
            expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
                default;
            expected_delete_assign_base&
                operator=(const expected_delete_assign_base&) = default;
            expected_delete_assign_base&
                operator=(expected_delete_assign_base&&) noexcept = default;
        };

        template <class T, class E>
        struct expected_delete_assign_base<T, E, true, false> {
            expected_delete_assign_base() = default;
            expected_delete_assign_base(const expected_delete_assign_base&) = default;
            expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
                default;
            expected_delete_assign_base&
                operator=(const expected_delete_assign_base&) = default;
            expected_delete_assign_base&
                operator=(expected_delete_assign_base&&) noexcept = delete;
        };

        template <class T, class E>
        struct expected_delete_assign_base<T, E, false, true> {
            expected_delete_assign_base() = default;
            expected_delete_assign_base(const expected_delete_assign_base&) = default;
            expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
                default;
            expected_delete_assign_base&
                operator=(const expected_delete_assign_base&) = delete;
            expected_delete_assign_base&
                operator=(expected_delete_assign_base&&) noexcept = default;
        };

        template <class T, class E>
        struct expected_delete_assign_base<T, E, false, false> {
            expected_delete_assign_base() = default;
            expected_delete_assign_base(const expected_delete_assign_base&) = default;
            expected_delete_assign_base(expected_delete_assign_base&&) noexcept =
                default;
            expected_delete_assign_base&
                operator=(const expected_delete_assign_base&) = delete;
            expected_delete_assign_base&
                operator=(expected_delete_assign_base&&) noexcept = delete;
        };

        // ����́A���̌�ɑ��� expected_default_ctor_base ���A�f�t�H���g�R���X�g���N�^��
        // �����t���ō폜���Ȃ���\�z�ł���悤�ɂ��邽�߂ɕK�v�ł��B
        struct default_constructor_tag {
            explicit constexpr default_constructor_tag() = default;
        };

        // expected_default_ctor_base �́AT���f�t�H���g�ō\���\�łȂ��ꍇ�ɁA
        // ���҂����f�t�H���g�̃R���X�g���N�^���폜����邱�Ƃ�ۏ؂��܂��B
        // ���̓��ꉻ�́AT ���f�t�H���g�ō\���\�ł���Ƃ��̂��߂ɂ���܂��B
        template <class T, class E,
            bool Enable =
            std::is_default_constructible<T>::value || std::is_void<T>::value>
            struct expected_default_ctor_base {
            constexpr expected_default_ctor_base() noexcept = default;
            constexpr expected_default_ctor_base(
                expected_default_ctor_base const&) noexcept = default;
            constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept =
                default;
            expected_default_ctor_base&
                operator=(expected_default_ctor_base const&) noexcept = default;
            expected_default_ctor_base&
                operator=(expected_default_ctor_base&&) noexcept = default;

            constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
        };

        // ���̓��ꉻ�́AT ���f�t�H���g�ō\���\�łȂ��ꍇ�̂��߂ɂ���܂��B
        template <class T, class E> struct expected_default_ctor_base<T, E, false> {
            constexpr expected_default_ctor_base() noexcept = delete;
            constexpr expected_default_ctor_base(
                expected_default_ctor_base const&) noexcept = default;
            constexpr expected_default_ctor_base(expected_default_ctor_base&&) noexcept =
                default;
            expected_default_ctor_base&
                operator=(expected_default_ctor_base const&) noexcept = default;
            expected_default_ctor_base&
                operator=(expected_default_ctor_base&&) noexcept = default;

            constexpr explicit expected_default_ctor_base(default_constructor_tag) {}
        };

    } // namespace detail

    template <class E> class bad_expected_access : public std::exception {
    public:
        explicit bad_expected_access(E e) : m_val(std::move(e)) {}

        virtual const char* what() const noexcept override {
            return "Bad expected access";
        }

        const E& error() const& { return m_val; }
        E& error()& { return m_val; }
        const E&& error() const&& { return std::move(m_val); }
        E&& error()&& { return std::move(m_val); }

    private:
        E m_val;
    };

    //=================
    /// An `expected<T, E>` object is an object that contains the storage for
    /// another object and manages the lifetime of this contained object `T`.
    /// Alternatively it could contain the storage for another unexpected object
    /// `E`. The contained object may not be initialized after the expected object
    /// has been initialized, and may not be destroyed before the expected object
    /// has been destroyed. The initialization state of the contained object is
    /// tracked by the expected object.
    template <class T, class E>
    class expected :
        private detail::expected_move_assign_base<T, E>,
        private detail::expected_delete_ctor_base<T, E>,
        private detail::expected_delete_assign_base<T, E>,
        private detail::expected_default_ctor_base<T, E> {
        static_assert(!std::is_reference<T>::value, "T must not be a reference");
        static_assert(!std::is_same<T, std::remove_cv<in_place_t>::type>::value,
            "T must not be in_place_t");
        static_assert(!std::is_same<T, std::remove_cv<unexpect_t>::type>::value,
            "T must not be unexpect_t");
        static_assert(!std::is_same<T, typename std::remove_cv<unexpected<E>>::type>::value,
            "T must not be unexpected<E>");
        static_assert(!std::is_reference<E>::value, "E must not be a reference");

        T* valptr() { return std::addressof(this->m_val); }
        const T* valptr() const { return std::addressof(this->m_val); }
        unexpected<E>* errptr() { return std::addressof(this->m_unexpect); }
        const unexpected<E>* errptr() const { return std::addressof(this->m_unexpect); }

        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr U& val() {
            return this->m_val;
        }
        constexpr unexpected<E>& err() { return this->m_unexpect; }

        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr const U& val() const {
            return this->m_val;
        }
        constexpr const unexpected<E>& err() const { return this->m_unexpect; }

        using impl_base = detail::expected_move_assign_base<T, E>;
        using ctor_base = detail::expected_default_ctor_base<T, E>;

    public:
        typedef T value_type;
        typedef E error_type;
        typedef unexpected<E> unexpected_type;

        template <class F> constexpr auto and_then(F&& f)& {
            return and_then_impl(*this, std::forward<F>(f));
        }
        template <class F> constexpr auto and_then(F&& f)&& {
            return and_then_impl(std::move(*this), std::forward<F>(f));
        }
        template <class F> constexpr auto and_then(F&& f) const& {
            return and_then_impl(*this, std::forward<F>(f));
        }

        template <class F> constexpr auto and_then(F&& f) const&& {
            return and_then_impl(std::move(*this), std::forward<F>(f));
        }

        template <class F> TL_EXPECTED_11_CONSTEXPR auto map(F&& f)& {
            return expected_map_impl(*this, std::forward<F>(f));
        }
        template <class F> TL_EXPECTED_11_CONSTEXPR auto map(F&& f)&& {
            return expected_map_impl(std::move(*this), std::forward<F>(f));
        }
        template <class F> constexpr auto map(F&& f) const& {
            return expected_map_impl(*this, std::forward<F>(f));
        }
        template <class F> constexpr auto map(F&& f) const&& {
            return expected_map_impl(std::move(*this), std::forward<F>(f));
        }

        template <class F> TL_EXPECTED_11_CONSTEXPR auto transform(F&& f)& {
            return expected_map_impl(*this, std::forward<F>(f));
        }
        template <class F> TL_EXPECTED_11_CONSTEXPR auto transform(F&& f)&& {
            return expected_map_impl(std::move(*this), std::forward<F>(f));
        }
        template <class F> constexpr auto transform(F&& f) const& {
            return expected_map_impl(*this, std::forward<F>(f));
        }
        template <class F> constexpr auto transform(F&& f) const&& {
            return expected_map_impl(std::move(*this), std::forward<F>(f));
        }

        template <class F> TL_EXPECTED_11_CONSTEXPR auto map_error(F&& f)& {
            return map_error_impl(*this, std::forward<F>(f));
        }
        template <class F> TL_EXPECTED_11_CONSTEXPR auto map_error(F&& f)&& {
            return map_error_impl(std::move(*this), std::forward<F>(f));
        }
        template <class F> constexpr auto map_error(F&& f) const& {
            return map_error_impl(*this, std::forward<F>(f));
        }
        template <class F> constexpr auto map_error(F&& f) const&& {
            return map_error_impl(std::move(*this), std::forward<F>(f));
        }

        template <class F> expected TL_EXPECTED_11_CONSTEXPR or_else(F&& f)& {
            return or_else_impl(*this, std::forward<F>(f));
        }

        template <class F> expected TL_EXPECTED_11_CONSTEXPR or_else(F&& f)&& {
            return or_else_impl(std::move(*this), std::forward<F>(f));
        }

        template <class F> expected constexpr or_else(F&& f) const& {
            return or_else_impl(*this, std::forward<F>(f));
        }

        constexpr expected() = default;
        constexpr expected(const expected& rhs) = default;
        constexpr expected(expected&& rhs) = default;
        expected& operator=(const expected& rhs) = default;
        expected& operator=(expected&& rhs) = default;

        template <class... Args,
            detail::enable_if_t<std::is_constructible<T, Args &&...>::value>* =
            nullptr>
            constexpr expected(in_place_t, Args &&... args)
            : impl_base(in_place, std::forward<Args>(args)...),
            ctor_base(detail::default_constructor_tag{}) {}

        template <class U, class... Args,
            detail::enable_if_t<std::is_constructible<
            T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr expected(in_place_t, std::initializer_list<U> il, Args &&... args)
            : impl_base(in_place, il, std::forward<Args>(args)...),
            ctor_base(detail::default_constructor_tag{}) {}

        template <class G = E,
            detail::enable_if_t<std::is_constructible<E, const G&>::value>* =
            nullptr,
            detail::enable_if_t<!std::is_convertible<const G&, E>::value>* =
            nullptr>
            explicit constexpr expected(const unexpected<G>& e)
            : impl_base(unexpect, e.value()),
            ctor_base(detail::default_constructor_tag{}) {}

        template <
            class G = E,
            detail::enable_if_t<std::is_constructible<E, const G&>::value>* =
            nullptr,
            detail::enable_if_t<std::is_convertible<const G&, E>::value>* = nullptr>
            constexpr expected(unexpected<G> const& e)
            : impl_base(unexpect, e.value()),
            ctor_base(detail::default_constructor_tag{}) {}

        template <
            class G = E,
            detail::enable_if_t<std::is_constructible<E, G&&>::value>* = nullptr,
            detail::enable_if_t<!std::is_convertible<G&&, E>::value>* = nullptr>
            explicit constexpr expected(unexpected<G>&& e) noexcept(
                std::is_nothrow_constructible<E, G&&>::value)
            : impl_base(unexpect, std::move(e.value())),
            ctor_base(detail::default_constructor_tag{}) {}

        template <
            class G = E,
            detail::enable_if_t<std::is_constructible<E, G&&>::value>* = nullptr,
            detail::enable_if_t<std::is_convertible<G&&, E>::value>* = nullptr>
            constexpr expected(unexpected<G>&& e) noexcept(
                std::is_nothrow_constructible<E, G&&>::value)
            : impl_base(unexpect, std::move(e.value())),
            ctor_base(detail::default_constructor_tag{}) {}

        template <class... Args,
            detail::enable_if_t<std::is_constructible<E, Args &&...>::value>* =
            nullptr>
            constexpr explicit expected(unexpect_t, Args &&... args)
            : impl_base(unexpect, std::forward<Args>(args)...),
            ctor_base(detail::default_constructor_tag{}) {}

        template <class U, class... Args,
            detail::enable_if_t<std::is_constructible<
            E, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            constexpr explicit expected(unexpect_t, std::initializer_list<U> il,
                Args &&... args)
            : impl_base(unexpect, il, std::forward<Args>(args)...),
            ctor_base(detail::default_constructor_tag{}) {}

        template <class U, class G,
            detail::enable_if_t<!(std::is_convertible<U const&, T>::value&&
                std::is_convertible<G const&, E>::value)>* =
            nullptr,
            detail::expected_enable_from_other<T, E, U, G, const U&, const G&>
            * = nullptr>
            explicit constexpr expected(const expected<U, G>& rhs)
            : ctor_base(detail::default_constructor_tag{}) {
            if (rhs.has_value()) {
                this->construct(*rhs);
            }
            else {
                this->construct_error(rhs.error());
            }
        }

        template <class U, class G,
            detail::enable_if_t<(std::is_convertible<U const&, T>::value&&
                std::is_convertible<G const&, E>::value)>* =
            nullptr,
            detail::expected_enable_from_other<T, E, U, G, const U&, const G&>
            * = nullptr>
            constexpr expected(const expected<U, G>& rhs)
            : ctor_base(detail::default_constructor_tag{}) {
            if (rhs.has_value()) {
                this->construct(*rhs);
            }
            else {
                this->construct_error(rhs.error());
            }
        }

        template <
            class U, class G,
            detail::enable_if_t<!(std::is_convertible<U&&, T>::value&&
                std::is_convertible<G&&, E>::value)>* = nullptr,
            detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
            explicit constexpr expected(expected<U, G>&& rhs)
            : ctor_base(detail::default_constructor_tag{}) {
            if (rhs.has_value()) {
                this->construct(std::move(*rhs));
            }
            else {
                this->construct_error(std::move(rhs.error()));
            }
        }

        template <
            class U, class G,
            detail::enable_if_t<(std::is_convertible<U&&, T>::value&&
                std::is_convertible<G&&, E>::value)>* = nullptr,
            detail::expected_enable_from_other<T, E, U, G, U&&, G&&>* = nullptr>
            TL_EXPECTED_11_CONSTEXPR expected(expected<U, G>&& rhs)
            : ctor_base(detail::default_constructor_tag{}) {
            if (rhs.has_value()) {
                this->construct(std::move(*rhs));
            }
            else {
                this->construct_error(std::move(rhs.error()));
            }
        }

        // �^T���E�Ӓl�Q�ƂֈÖقȌ^�ϊ����ł��Ȃ��ꍇ
        // ���R���p�C���G���[�ɂȂ�
        template <
            class U = T,
            detail::enable_if_t<!std::is_convertible<U&&, T>::value>* = nullptr,
            detail::expected_enable_forward_value<T, E, U>* = nullptr>
            explicit TL_EXPECTED_MSVC2015_CONSTEXPR expected(U&& v)
            : expected(in_place, std::forward<U>(v)) {}

        // �^T���E�Ӓl�Q�ƂֈÖقȌ^�ϊ��ł���ꍇ
        template <
            class U = T,
            detail::enable_if_t<std::is_convertible<U&&, T>::value>* = nullptr,
            detail::expected_enable_forward_value<T, E, U>* = nullptr>
            TL_EXPECTED_MSVC2015_CONSTEXPR expected(U&& v)
            : expected(in_place, std::forward<U>(v)) {}

        template <
            class U = T, class G = T,
            detail::enable_if_t<std::is_nothrow_constructible<T, U&&>::value>* =
            nullptr,
            detail::enable_if_t<!std::is_void<G>::value>* = nullptr,
            detail::enable_if_t<
            (!std::is_same<expected<T, E>, detail::decay_t<U>>::value &&
                !detail::conjunction<std::is_scalar<T>,
                std::is_same<T, detail::decay_t<U>>>::value&&
                std::is_constructible<T, U>::value&&
                std::is_assignable<G&, U>::value&&
                std::is_nothrow_move_constructible<E>::value)>* = nullptr>
            expected& operator=(U&& v) {
            if (has_value()) {
                val() = std::forward<U>(v);
            }
            else {
                err().~unexpected<E>();
                ::new (valptr()) T(std::forward<U>(v));
                this->m_has_val = true;
            }

            return *this;
        }

        template <
            class U = T, class G = T,
            detail::enable_if_t<!std::is_nothrow_constructible<T, U&&>::value>* =
            nullptr,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr,
            detail::enable_if_t<
            (!std::is_same<expected<T, E>, detail::decay_t<U>>::value &&
                !detail::conjunction<std::is_scalar<T>,
                std::is_same<T, detail::decay_t<U>>>::value&&
                std::is_constructible<T, U>::value&&
                std::is_assignable<G&, U>::value&&
                std::is_nothrow_move_constructible<E>::value)>* = nullptr>
            expected& operator=(U&& v) {
            if (has_value()) {
                val() = std::forward<U>(v);
            }
            else {
                auto tmp = std::move(err());
                err().~unexpected<E>();

#ifdef EXPECTED_EXCEPTIONS_ENABLED
                try {
                    ::new (valptr()) T(std::forward<U>(v));
                    this->m_has_val = true;
                }
                catch (...) {
                    err() = std::move(tmp);
                    throw;
                }
#else
                ::new (valptr()) T(std::forward<U>(v));
                this->m_has_val = true;
#endif
            }

            return *this;
        }

        template <class G = E,
            detail::enable_if_t<std::is_nothrow_copy_constructible<G>::value&&
            std::is_assignable<G&, G>::value>* = nullptr>
            expected& operator=(const unexpected<G>& rhs) {
            if (!has_value()) {
                err() = rhs;
            }
            else {
                this->destroy_val();
                ::new (errptr()) unexpected<E>(rhs);
                this->m_has_val = false;
            }

            return *this;
        }

        template <class G = E,
            detail::enable_if_t<std::is_nothrow_move_constructible<G>::value&&
            std::is_move_assignable<G>::value>* = nullptr>
            expected& operator=(unexpected<G>&& rhs) noexcept {
            if (!has_value()) {
                err() = std::move(rhs);
            }
            else {
                this->destroy_val();
                ::new (errptr()) unexpected<E>(std::move(rhs));
                this->m_has_val = false;
            }

            return *this;
        }

        template <class... Args, detail::enable_if_t<std::is_nothrow_constructible<
            T, Args &&...>::value>* = nullptr>
            void emplace(Args &&... args) {
            if (has_value()) {
                val() = T(std::forward<Args>(args)...);
            }
            else {
                err().~unexpected<E>();
                ::new (valptr()) T(std::forward<Args>(args)...);
                this->m_has_val = true;
            }
        }

        template <class... Args, detail::enable_if_t<!std::is_nothrow_constructible<
            T, Args &&...>::value>* = nullptr>
            void emplace(Args &&... args) {
            if (has_value()) {
                val() = T(std::forward<Args>(args)...);
            }
            else {
                auto tmp = std::move(err());
                err().~unexpected<E>();

#ifdef EXPECTED_EXCEPTIONS_ENABLED
                try {
                    ::new (valptr()) T(std::forward<Args>(args)...);
                    this->m_has_val = true;
                }
                catch (...) {
                    err() = std::move(tmp);
                    throw;
                }
#else
                ::new (valptr()) T(std::forward<Args>(args)...);
                this->m_has_val = true;
#endif
            }
        }

        template <class U, class... Args,
            detail::enable_if_t<std::is_nothrow_constructible<
            T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            void emplace(std::initializer_list<U> il, Args &&... args) {
            if (has_value()) {
                T t(il, std::forward<Args>(args)...);
                val() = std::move(t);
            }
            else {
                err().~unexpected<E>();
                ::new (valptr()) T(il, std::forward<Args>(args)...);
                this->m_has_val = true;
            }
        }

        template <class U, class... Args,
            detail::enable_if_t<!std::is_nothrow_constructible<
            T, std::initializer_list<U>&, Args &&...>::value>* = nullptr>
            void emplace(std::initializer_list<U> il, Args &&... args) {
            if (has_value()) {
                T t(il, std::forward<Args>(args)...);
                val() = std::move(t);
            }
            else {
                auto tmp = std::move(err());
                err().~unexpected<E>();

#ifdef EXPECTED_EXCEPTIONS_ENABLED
                try {
                    ::new (valptr()) T(il, std::forward<Args>(args)...);
                    this->m_has_val = true;
                }
                catch (...) {
                    err() = std::move(tmp);
                    throw;
                }
#else
                ::new (valptr()) T(il, std::forward<Args>(args)...);
                this->m_has_val = true;
#endif
            }
        }

    private:
        using t_is_void = std::true_type;
        using t_is_not_void = std::false_type;
        using t_is_nothrow_move_constructible = std::true_type;
        using move_constructing_t_can_throw = std::false_type;
        using e_is_nothrow_move_constructible = std::true_type;
        using move_constructing_e_can_throw = std::false_type;

        void swap_where_both_have_value(expected&/*rhs*/, t_is_void) noexcept {
            // swapping void is a no-op
        }

        void swap_where_both_have_value(expected& rhs, t_is_not_void) {
            using std::swap;
            swap(val(), rhs.val());
        }

        void swap_where_only_one_has_value(expected& rhs, t_is_void) noexcept(
            std::is_nothrow_move_constructible<E>::value) {
            ::new (errptr()) unexpected_type(std::move(rhs.err()));
            rhs.err().~unexpected_type();
            std::swap(this->m_has_val, rhs.m_has_val);
        }

        void swap_where_only_one_has_value(expected& rhs, t_is_not_void) {
            swap_where_only_one_has_value_and_t_is_not_void(
                rhs, typename std::is_nothrow_move_constructible<T>::type{},
                typename std::is_nothrow_move_constructible<E>::type{});
        }

        void swap_where_only_one_has_value_and_t_is_not_void(
            expected& rhs, t_is_nothrow_move_constructible,
            e_is_nothrow_move_constructible) noexcept {
            auto temp = std::move(val());
            val().~T();
            ::new (errptr()) unexpected_type(std::move(rhs.err()));
            rhs.err().~unexpected_type();
            ::new (rhs.valptr()) T(std::move(temp));
            std::swap(this->m_has_val, rhs.m_has_val);
        }

        void swap_where_only_one_has_value_and_t_is_not_void(
            expected& rhs, t_is_nothrow_move_constructible,
            move_constructing_e_can_throw) {
            auto temp = std::move(val());
            val().~T();
#ifdef EXPECTED_EXCEPTIONS_ENABLED
            try {
                ::new (errptr()) unexpected_type(std::move(rhs.err()));
                rhs.err().~unexpected_type();
                ::new (rhs.valptr()) T(std::move(temp));
                std::swap(this->m_has_val, rhs.m_has_val);
            }
            catch (...) {
                val() = std::move(temp);
                throw;
            }
#else
            ::new (errptr()) unexpected_type(std::move(rhs.err()));
            rhs.err().~unexpected_type();
            ::new (rhs.valptr()) T(std::move(temp));
            std::swap(this->m_has_val, rhs.m_has_val);
#endif
        }

        void swap_where_only_one_has_value_and_t_is_not_void(
            expected& rhs, move_constructing_t_can_throw,
            t_is_nothrow_move_constructible) {
            auto temp = std::move(rhs.err());
            rhs.err().~unexpected_type();
#ifdef EXPECTED_EXCEPTIONS_ENABLED
            try {
                ::new (rhs.valptr()) T(val());
                val().~T();
                ::new (errptr()) unexpected_type(std::move(temp));
                std::swap(this->m_has_val, rhs.m_has_val);
            }
            catch (...) {
                rhs.err() = std::move(temp);
                throw;
            }
#else
            ::new (rhs.valptr()) T(val());
            val().~T();
            ::new (errptr()) unexpected_type(std::move(temp));
            std::swap(this->m_has_val, rhs.m_has_val);
#endif
        }


    public:
        template <class OT = T, class OE = E>
        detail::enable_if_t<detail::is_swappable<OT>::value&&
            detail::is_swappable<OE>::value &&
            (std::is_nothrow_move_constructible<OT>::value ||
                std::is_nothrow_move_constructible<OE>::value)>
            swap(expected& rhs) noexcept(
                std::is_nothrow_move_constructible<T>::value
                && detail::is_nothrow_swappable<T>::value
                && std::is_nothrow_move_constructible<E>::value
                && detail::is_nothrow_swappable<E>::value) {
            if (has_value() && rhs.has_value()) {
                swap_where_both_have_value(rhs, typename std::is_void<T>::type{});
            }
            else if (!has_value() && rhs.has_value()) {
                rhs.swap(*this);
            }
            else if (has_value()) {
                swap_where_only_one_has_value(rhs, typename std::is_void<T>::type{});
            }
            else {
                using std::swap;
                swap(err(), rhs.err());
            }
        }

        constexpr const T* operator->() const { return valptr(); }
        constexpr T* operator->() { return valptr(); }

        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr const U& operator*() const& {
            return val();
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr U& operator*()& {
            return val();
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr const U&& operator*() const&& {
            return std::move(val());
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr U&& operator*()&& {
            return std::move(val());
        }

        constexpr bool has_value() const noexcept { return this->m_has_val; }
        constexpr explicit operator bool() const noexcept { return this->m_has_val; }

        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr const U& value() const& {
            if (!has_value())
                detail::throw_exception(bad_expected_access<E>(err().value()));
            return val();
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr U& value()& {
            if (!has_value())
                detail::throw_exception(bad_expected_access<E>(err().value()));
            return val();
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr const U&& value() const&& {
            if (!has_value())
                detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
            return std::move(val());
        }
        template <class U = T,
            detail::enable_if_t<!std::is_void<U>::value>* = nullptr>
            constexpr U&& value()&& {
            if (!has_value())
                detail::throw_exception(bad_expected_access<E>(std::move(err()).value()));
            return std::move(val());
        }

        constexpr const E& error() const& { return err().value(); }
        constexpr E& error()& { return err().value(); }
        constexpr const E&& error() const&& { return std::move(err().value()); }
        constexpr E&& error()&& { return std::move(err().value()); }

        template <class U> constexpr T value_or(U&& v) const& {
            static_assert(std::is_copy_constructible<T>::value &&
                std::is_convertible<U&&, T>::value,
                "T must be copy-constructible and convertible to from U&&");
            return bool(*this) ? **this : static_cast<T>(std::forward<U>(v));
        }
        template <class U> constexpr T value_or(U&& v)&& {
            static_assert(std::is_move_constructible<T>::value &&
                std::is_convertible<U&&, T>::value,
                "T must be move-constructible and convertible to from U&&");
            return bool(*this) ? std::move(**this) : static_cast<T>(std::forward<U>(v));
        }
    };

    namespace detail {
        template <class Exp> using exp_t = typename detail::decay_t<Exp>::value_type;
        template <class Exp> using err_t = typename detail::decay_t<Exp>::error_type;
        template <class Exp, class Ret> using ret_t = expected<Ret, err_t<Exp>>;

        template <class Exp, class F,
            detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                *std::declval<Exp>()))>
            constexpr auto and_then_impl(Exp&& exp, F&& f) {
            static_assert(detail::is_expected<Ret>::value, "F must return an expected");

            return exp.has_value()
                ? detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp))
                : Ret(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>()))>
            constexpr auto and_then_impl(Exp&& exp, F&& f) {
            static_assert(detail::is_expected<Ret>::value, "F must return an expected");

            return exp.has_value() ? detail::invoke(std::forward<F>(f))
                : Ret(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                *std::declval<Exp>())),
            detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
            constexpr auto expected_map_impl(Exp&& exp, F&& f) {
            using result = ret_t<Exp, detail::decay_t<Ret>>;
            return exp.has_value() ? result(detail::invoke(std::forward<F>(f),
                *std::forward<Exp>(exp)))
                : result(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                *std::declval<Exp>())),
            detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
            auto expected_map_impl(Exp&& exp, F&& f) {
            using result = expected<void, err_t<Exp>>;
            if (exp.has_value()) {
                detail::invoke(std::forward<F>(f), *std::forward<Exp>(exp));
                return result();
            }

            return result(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>())),
            detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
            constexpr auto expected_map_impl(Exp&& exp, F&& f) {
            using result = ret_t<Exp, detail::decay_t<Ret>>;
            return exp.has_value() ? result(detail::invoke(std::forward<F>(f)))
                : result(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>())),
            detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
            auto expected_map_impl(Exp&& exp, F&& f) {
            using result = expected<void, err_t<Exp>>;
            if (exp.has_value()) {
                detail::invoke(std::forward<F>(f));
                return result();
            }

            return result(unexpect, std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
            constexpr auto map_error_impl(Exp&& exp, F&& f) {
            using result = expected<exp_t<Exp>, detail::decay_t<Ret>>;
            return exp.has_value()
                ? result(*std::forward<Exp>(exp))
                : result(unexpect, detail::invoke(std::forward<F>(f),
                    std::forward<Exp>(exp).error()));
        }
        template <class Exp, class F,
            detail::enable_if_t<!std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
            auto map_error_impl(Exp&& exp, F&& f) {
            using result = expected<exp_t<Exp>, monostate>;
            if (exp.has_value()) {
                return result(*std::forward<Exp>(exp));
            }

            detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
            return result(unexpect, monostate{});
        }
        template <class Exp, class F,
            detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
            constexpr auto map_error_impl(Exp&& exp, F&& f) {
            using result = expected<exp_t<Exp>, detail::decay_t<Ret>>;
            return exp.has_value()
                ? result()
                : result(unexpect, detail::invoke(std::forward<F>(f),
                    std::forward<Exp>(exp).error()));
        }
        template <class Exp, class F,
            detail::enable_if_t<std::is_void<exp_t<Exp>>::value>* = nullptr,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
            auto map_error_impl(Exp&& exp, F&& f) {
            using result = expected<exp_t<Exp>, monostate>;
            if (exp.has_value()) {
                return result();
            }

            detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
            return result(unexpect, monostate{});
        }

        template <class Exp, class F,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<!std::is_void<Ret>::value>* = nullptr>
            constexpr auto or_else_impl(Exp&& exp, F&& f) {
            static_assert(detail::is_expected<Ret>::value, "F must return an expected");
            return exp.has_value()
                ? std::forward<Exp>(exp)
                : detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error());
        }

        template <class Exp, class F,
            class Ret = decltype(detail::invoke(std::declval<F>(),
                std::declval<Exp>().error())),
            detail::enable_if_t<std::is_void<Ret>::value>* = nullptr>
            detail::decay_t<Exp> or_else_impl(Exp&& exp, F&& f) {
            return exp.has_value()
                ? std::forward<Exp>(exp)
                : (detail::invoke(std::forward<F>(f), std::forward<Exp>(exp).error()),
                    std::forward<Exp>(exp));
        }
    } // namespace detail

    template <class T, class E, class U, class F>
    constexpr bool operator==(const expected<T, E>& lhs,
        const expected<U, F>& rhs) {
        return (lhs.has_value() != rhs.has_value())
            ? false
            : (!lhs.has_value() ? lhs.error() == rhs.error() : *lhs == *rhs);
    }
    template <class T, class E, class U, class F>
    constexpr bool operator!=(const expected<T, E>& lhs,
        const expected<U, F>& rhs) {
        return (lhs.has_value() != rhs.has_value())
            ? true
            : (!lhs.has_value() ? lhs.error() != rhs.error() : *lhs != *rhs);
    }

    template <class T, class E, class U>
    constexpr bool operator==(const expected<T, E>& x, const U& v) {
        return x.has_value() ? *x == v : false;
    }
    template <class T, class E, class U>
    constexpr bool operator==(const U& v, const expected<T, E>& x) {
        return x.has_value() ? *x == v : false;
    }
    template <class T, class E, class U>
    constexpr bool operator!=(const expected<T, E>& x, const U& v) {
        return x.has_value() ? *x != v : true;
    }
    template <class T, class E, class U>
    constexpr bool operator!=(const U& v, const expected<T, E>& x) {
        return x.has_value() ? *x != v : true;
    }

    template <class T, class E>
    constexpr bool operator==(const expected<T, E>& x, const unexpected<E>& e) {
        return x.has_value() ? false : x.error() == e.value();
    }
    template <class T, class E>
    constexpr bool operator==(const unexpected<E>& e, const expected<T, E>& x) {
        return x.has_value() ? false : x.error() == e.value();
    }
    template <class T, class E>
    constexpr bool operator!=(const expected<T, E>& x, const unexpected<E>& e) {
        return x.has_value() ? true : x.error() != e.value();
    }
    template <class T, class E>
    constexpr bool operator!=(const unexpected<E>& e, const expected<T, E>& x) {
        return x.has_value() ? true : x.error() != e.value();
    }

    template <
        class T,
        class E,
        detail::enable_if_t<(std::is_void<T>::value ||
            std::is_move_constructible<T>::value) &&
        detail::is_swappable<T>::value&&
        std::is_move_constructible<E>::value&&
        detail::is_swappable<E>::value>* = nullptr>
        void swap(expected<T, E>& lhs,
            expected<T, E>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }
}
