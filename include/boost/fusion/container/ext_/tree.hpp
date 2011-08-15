/*=============================================================================
   Copyright (c) 2006 Eric Niebler

   Use, modification and distribution is subject to the Boost Software
   License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef FUSION_BINARY_TREE_EAN_05032006_1027
#define FUSION_BINARY_TREE_EAN_05032006_1027

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>
#include <boost/fusion/support/is_sequence.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/view/single_view.hpp>
#include <boost/fusion/container/list/cons.hpp> // for nil
#include <boost/fusion/container/vector/vector10.hpp>
#include <boost/fusion/support/sequence_base.hpp>
#include <boost/fusion/support/category_of.hpp>
#include <boost/fusion/sequence/intrinsic/ext_/segments.hpp>
#include <boost/fusion/sequence/intrinsic/ext_/segmented_size.hpp>
#include <boost/fusion/support/ext_/is_segmented.hpp>
#include <boost/fusion/view/ext_/segmented_iterator.hpp>
#include <boost/fusion/view/ext_/segmented_begin.hpp>
#include <boost/fusion/view/ext_/segmented_end.hpp>

namespace boost { namespace fusion
{
   struct tree_tag;

   template<typename Data, typename Left = nil, typename Right = nil>
   struct tree
     : sequence_base<tree<Data, Left, Right> >
   {
       typedef Data data_type;
       typedef Left left_type;
       typedef Right right_type;
       typedef tree_tag fusion_tag;
       typedef bidirectional_traversal_tag category;
       typedef mpl::false_ is_view;

       typedef typename mpl::if_<
           traits::is_sequence<Data>
         , Data
         , single_view<Data>
       >::type data_view;

       explicit tree(
           typename fusion::detail::call_param<Data>::type data_
         , typename fusion::detail::call_param<Left>::type left_ = Left()
         , typename fusion::detail::call_param<Right>::type right_ = Right()
       )
         : segments(left_, data_view(data_), right_)
       {}

       typedef vector3<Left, data_view, Right> segments_type;
       segments_type segments;
   };

   template<typename Data>
   tree<Data> make_tree(Data const &data)
   {
       return tree<Data>(data);
   }

   template<typename Data, typename Left, typename Right>
   tree<Data, Left, Right> make_tree(Data const &data, Left const &left, Right const &right)
   {
       return tree<Data, Left, Right>(data, left, right);
   }

   namespace extension
   {
       template<>
       struct is_segmented_impl<tree_tag>
       {
           template<typename Sequence>
           struct apply : mpl::true_ {};
       };

       template<>
       struct segments_impl<tree_tag>
       {
           template<typename Sequence>
           struct apply
           {
               typedef typename mpl::if_<
                   is_const<Sequence>
                 , typename Sequence::segments_type const &
                 , typename Sequence::segments_type &
               >::type type;

               static type call(Sequence &seq)
               {
                   return seq.segments;
               }
           };
       };

       template<>
       struct begin_impl<tree_tag>
       {
           template<typename Sequence>
           struct apply
             : segmented_begin<Sequence>
           {};
       };

       template<>
       struct end_impl<tree_tag>
       {
           template<typename Sequence>
           struct apply
             : segmented_end<Sequence>
           {};
       };

       template<>
       struct size_impl<tree_tag>
       {
           template<typename Sequence>
           struct apply
             : segmented_size<Sequence>::type
           {};
       };
   }
}}

#endif
