
class Container { /* Size=0xe0 */
  /* 0x0008 */ ContainerType mContainerType;
  /* 0x0009 */ ContainerType mGameplayContainerType;
  /* 0x0010 */ std::unordered_set<ContainerContentChangeListener *,std::hash<ContainerContentChangeListener *>,std::equal_to<ContainerContentChangeListener *>,std::allocator<ContainerContentChangeListener *> > mContentChangeListeners;
  /* 0x0050 */ std::unordered_set<ContainerSizeChangeListener *,std::hash<ContainerSizeChangeListener *>,std::equal_to<ContainerSizeChangeListener *>,std::allocator<ContainerSizeChangeListener *> > mSizeChangeListeners;
  /* 0x0090 */ std::deque<std::function<void __cdecl(Container &,int,ItemStack const &,ItemStack const &)>,std::allocator<std::function<void __cdecl(Container &,int,ItemStack const &,ItemStack const &)> > > mTransactionContextStack;
  /* 0x00b8 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mName;
  /* 0x00d8 */ bool mCustomName;
  /* 0x00dc */ TypedRuntimeId<ContainerRuntimeIdTag,unsigned int,0> mContainerRuntimeId;
  static const int32_t LARGE_MAX_STACK_SIZE;
  static const int32_t DEFAULT_CONTAINER_SIZE;
  static const int32_t INVALID_SLOT;
  static const uint32_t INVALID_CONTAINER_ID;
  static const BidirectionalUnorderedMap<enum ContainerType,std::basic_string<char,std::char_traits<char>,std::allocator<char> > > containerTypeMap;
  
  Container(const Container&);
  Container(ContainerType, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, bool);
  Container(ContainerType);
  virtual ~Container();
  virtual void init();
  void initRuntimeId();
  const TypedRuntimeId<ContainerRuntimeIdTag,unsigned int,0>& getRuntimeId() const;
  virtual void serverInitItemStackIds(int32_t, int32_t, std::function<void __cdecl(int,ItemStack const &)>);
  void serverInitItemStackId(int32_t, std::function<void __cdecl(int,ItemStack const &)>);
  void serverInitItemStackIdsAll(std::function<void __cdecl(int,ItemStack const &)>);
  void addSizeChangeListener(ContainerSizeChangeListener*);
  void removeSizeChangeListener(ContainerSizeChangeListener*);
  virtual void addContentChangeListener(ContainerContentChangeListener*);
  virtual void removeContentChangeListener(ContainerContentChangeListener*);
  virtual const ItemStack& getItem(int32_t) const;
  virtual bool hasRoomForItem(const ItemStack&);
  virtual void addItem(ItemStack&);
  virtual bool addItemToFirstEmptySlot(ItemStack&);
  virtual void setItem(int32_t, const ItemStack&);
  virtual void setItemWithForceBalance(int32_t, const ItemStack&, bool);
  virtual void removeItem(int32_t, int32_t);
  virtual void removeAllItems();
  virtual void dropContents(BlockSource&, const Vec3&, bool);
  virtual int32_t getContainerSize() const;
  virtual int32_t getMaxStackSize() const;
  virtual void startOpen(Player&);
  virtual void stopOpen(Player&);
  virtual std::vector<ItemStack,std::allocator<ItemStack> > getSlotCopies() const;
  virtual const std::vector<ItemStack const *,std::allocator<ItemStack const *> > getSlots() const;
  virtual int32_t getItemCount(const ItemStack&) const;
  int32_t getItemCount(const ItemDescriptor&);
  int32_t getItemCount(const std::function<bool __cdecl(ItemStack const &)>);
  virtual int32_t findFirstSlotForItem(const ItemStack&) const;
  virtual bool canPushInItem(BlockSource&, int32_t, int32_t, const ItemInstance&) const;
  virtual bool canPullOutItem(BlockSource&, int32_t, int32_t, const ItemInstance&) const;
  ContainerType getContainerType() const;
  ContainerType getGameplayContainerType() const;
  void setGameplayContainerType(ContainerType);
  int32_t getRedstoneSignalFromContainer(BlockSource&);
  const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getName() const;
  void setSizeChanged(int32_t);
  virtual void setContainerChanged(int32_t);
  virtual void setContainerMoved();
  virtual void setCustomName(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  virtual bool hasCustomName() const;
  virtual void readAdditionalSaveData(const CompoundTag&);
  virtual void addAdditionalSaveData(CompoundTag&);
  virtual void createTransactionContext(std::function<void __cdecl(Container &,int,ItemStack const &,ItemStack const &)>, std::function<void __cdecl(void)>);
  void triggerTransactionChange(int32_t, const ItemStack&, const ItemStack&);
  virtual void initializeContainerContents(BlockSource&);
  virtual bool isEmpty() const;
  void _serverInitId(int32_t, ItemStack&, std::function<void __cdecl(int,ItemStack const &)>);
  void _initRuntimeId(const TypedRuntimeId<ContainerRuntimeIdTag,unsigned int,0>&);
  Container& operator=(const Container&);
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
  
  static const std::basic_string<char,std::char_traits<char>,std::allocator<char> >& getContainerTypeName(ContainerType);
  static ContainerType getContainerTypeId(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static std::function<bool __cdecl(ItemStack const &)> sameItemAndAuxComparator(const ItemDescriptor&);
};
