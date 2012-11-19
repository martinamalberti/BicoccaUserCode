#ifndef VBFMCChannelFilter_h
{
   public:

      explicit VBFMCChannelFilter (const edm::ParameterSet&) ;

   private:
      
      edm::InputTag m_MCtruthInputTag ;
      //! the decay channel: 
      //! * 1 = uu
      //! * 2 = ee
      //! * 3 = eu
      int m_channel ;

};
       
#endif
