1. 相比如mysql，redis也可以当作数据库来使用，而且更快，因为是内存中的数据库，但是和mysql相比，具有存储空间小
的问题，因为内存空间远小于硬盘空间
2. “二八原则”：20%的数据满足80%的访问需求，典型的方案：redis结合mysql使用。其中涉及系统中复杂度的增加、redis和mysql数据的同步问题等
3. redis最初是想作为一个消息队列（实现分布式系统中的生产者、消费者模型）
4. msyql数据库的主从分离与读写分离：读的需求是要远大于写的需求的，所以只有主库接受写的请求，并将发生改变的内容同步到从库，从库不负责处理写的请求，负责读的请求，上层也可以引入负载均衡的策略对读节点进行选择
# redis存放、获取键值的操作
## 1. 存放键值对
(1) redis中，key都是字符串，value可以是各种类型。存放键值对，使用set keyname keyvalue；redis不区分大小写，也不需要
注意单双引号的问题，写不写都可以。
(2) set keyname value [ex seconds | px milliseconds] [nx|xx]，ex指定超时时间以秒为单位，px以毫秒为单位；nx表示
(3) mset keyname value [keyname value] 设置多组键值对
(4) setnx setex psetex
有设置keyname则不插入，xx表示只在keyname存在时修改value
## 2. 获取键值对
(1) redis中，获取键值对使用get，通过get keyname的方式获取
(2) mget，获取多组键值对
### 获取键值对时的匹配原则
* <strong>?</strong>表示任意一个字符，如key可由ke?检索出来
* <strong>*</strong>表示任意多个字符，使用其实和Linux检索文件名时使用一样
* <strong>[abcdefg]</strong>表示从中括号中的选项中匹配任意一个
* <strong>[^e]</strong>表示不匹配e
* <strong>[a-c]</strong>表示范围匹配
<p>但是实际开发中，几乎不会使用（禁止）*的方式，因为会检索整个redis中的所有key，而redis又是单线程的，
所以可能会导致该线程阻塞在检索的过程中，导致大量请求从redis的缓存数据中取不出结果，都去向数据库发出请求，
导致数据库压力过大挂掉</p>

## 3. 查看键值对是否存在
<p>查看一个键值对（多个）是否存在，使用exists keyname1 keyname2 ...</p>
<p>关于同时支持多个keyname的问题：redis是客户端、服务器形式的应用，所以对于服务器的请求操作都要通过
网络进行，这意味着经由网络传输、各个层级之间协议的封装与解包，都是较大的时间消耗，所以redis支持一次
请求携带多个key值去操作，以节省传输的时间（网络的速度一般网卡和硬盘速度对比还是网络慢很多的</p>

## 4. 删除键值对
<p>使用del删除键值对，del keyname1 keyname2</p>

## 5. 设置过期时间
1. 秒为单位，expire keyname seconds
2. 毫秒为单位，pexpire keyname microseconds
## 6. 查看存活时间
1. ttl(time to live) + keyname，查看剩余存活时间，如果没有设置过期时间，那么就返回-1；如果查无此key，
就返回-2
## Redis怎样删除已经过期的key呢？
1. 定期删除。定期查看一部分key是否过期，这个过程很快（因为redis是单线程的，耗费时间过长就会导致类似于
前面说的*通配符使用的问题
2. 惰性删除，当一个key过期了，也不进行删除，而是在使用设个key的时候，发现这个key过期了，然后再删除
3. Redis其实并没有使用定时器的方式去删除过期的key，有些猜测是早期的redis是单线程的，便奠定了单线程
的基调，后面也维持了这种风格，但是定时器的实现方案，也是值得了解的，有优先级队列和时间轮两种方式
### 优先级队列
1. 将有过期时间的任务和过期时间放在一个根据过期时间建立的小根堆当中，根据堆顶元素的过期时间，去让
线程休眠一段时间，然后唤醒线程节省cpu资源；如果期间有新的事件进入队列，那么就再次唤醒线程检查是否需要
处理堆顶事件
### 时间轮
首先搭建一个环形队列，其中每个节点都指向一个链表的头节点，从头节点往后的每个节点都代表过期时间处在
某个范围的事件，让检测线程每个一段事件往后走一个检查该格中是否有需要处理的事件（结合周期和环形队列
循环的概念，不难得出一个链表中可能有多种时间），但是这种方式对于时间粒度和环形队列的长度的设计
都有很高的要求
## 7. 查看类型
使用type + keyname查看key对应的value的类型
# redis数据类型与实现方式
<p>很多类型，redis承诺对外是某种类型，但是具体实现的时候可能采用了其他的方式</p>

## 1. string
(1) raw，就是字符串的形式存储<br> 
(2) int，如果字符串中存储的是数字的话，就直接作为数字存储<br>
(3) embstr，对于短字符串做出优化的存储方式，也负责存储小数，这也意味着性能的损失，因为整数通过int的方式存储直接就可以
比较，但是通过embstr的话就要转化为数字之后再进行比较
## 2. hash
(1) hashtable，以redis自己的方式做的哈希表<br>
(2) ziplist，压缩表，当value的类型是哈希表但是其中的元素又很少时，就可以采用ziplist来遍历（因为元素少所以时间差异不大）压缩表通过内部的优化，将数据按照更紧凑的方式进行表示，当数据增多时，会导致效率下降
## 3. list
(1) 以前的版本采用的是linkedlist + ziplist实现，redis3.2以后，采取quicklist的实现方式，类似于std::deque
## 4. set
(1) hashtable (2) intset，当集合中只存储整数的时候，就是用这种结构
## 5. zset
(1) skiplist，跳表，类似于链表，但是每个节点有多个指针域，使之能够实现logN的查询效率
(2) ziplist
## 查看具体存储方式
object encoding keyname，查看某个value具体的存储方式
# redis的单线程工作模型
1. redis的客户端发送请求是并发的，但是redis服务器的核心工作逻辑是单线程的，这样请求也就再redis服务器上串行执行，
没有并发的问题
2. 所以说一方面，如果一个操作阻塞过久，就可能导致redis服务端挂掉；另一方面，也不是所有业务场景都适合单线程，因为
redis需要处理的业务基本上都是短平快的，对于cpu并行的要求很低，也就可以采用单线程的方式处理核心业务
# 为什么说redis快，效率高呢？
1. 首先redis效率高是和mysql这种关系型数据库相比，因为后者是对磁盘的操作，前者是对内存的操作，硬件上的操作速度
就差了几个数量级，因此redis自然更快
2. redis提供的功能相对要少一些，比如mysql插入的时候如果有约束的话还要射击先查询的过程，查询的时候还有多表联查
等等操作，在功能和业务上的区别，也是造成差异的原因
# string 类型
## 1. incr和incrby
(1) incr，表示给相应value + 1，只有实际类型是int的才可以<br>
(2) incrby，表示给相应value + n，n必须是整数，可以是负数
(3) 如果key不存在，就从零开始处理
## 2. decr和decrby
(1) 除了进行的是减操作以外，其他的都是和incr与incrby一样的
## 3. incrbyfloat
可以进行小数和整数的操作，如果需要减操作，使用负数即可
## 4. append
(1) 对字符串进行追加操作，并返回追加之后的长度 <br>
(2) 对于redis来说，不会对我们的数据进行字符集上的处理，因此输入中文的话默认打出的就是原编码，所以可以在启动客户端
的时候加上--raw指令，让客户端尝试解读返回的二进制数据并展示，就能够完成中文的显示了
## 5. getrange
(1) getrange keyname start end，表示获取[start, end]的内容，以字节为单位，这意味着可能会出现中文被解析成乱码的情况
，因为无论是utf8还是gbk都是多字节编码一个中文字符 <br>
(2) 当end是负数时，表示倒数第几个字符，例如-1表示倒数第一个字符 <br>
(3) 根据两个端点的情况，redis会自动截取内容（数字同理），但是如果最后出现start > end时，会导致解析不出内容
## 6. setrange
(1) setrange key start value，设置从start开始的内容为value，具体设置到哪里取决于value的长度<br>
(2) 如果设置的key不存在，那么就根据start的值，用空格填充前面的内容；如果start超过了当前内容的长度，也是同理往后
添加用空格补全长度
## 7. strlen
(1) 获取字符串类型的value的长度，单位是字节，对于中文也是具体字节而不是字符的个数
# Redis中的hash类型
redis中的hash，使用field-value表示键值的结构，其中，value的类型只允许是字符串
## 1. hset、hget、hexists、hdel
(1) hset keyname fieldname value，表示keyname用来查找这个哈希表，fieldname是哈希表的键值。
(2) hget keyname fieldname，获取keyname的哈希表的fieldname所对应的值
(3) hexists keyname fieldname，判断keyname对应的哈希表是否具有fieldname的键值
(4) hdel keyname fieldname，删除keyname对应的哈希表的fieldname键值
## 2. hkeys、hvals
hkeys key，获取key对应的哈希表中的所有field；hvals key，获取key对应的哈希表中的所有value值。尽量不要使用这些操作，
和keys *有异曲同工之妙
## hlen、hsetnx、hincrby、hincrbyfloat
1. hlen keyname，用来查询哈希表的大小<br>
2. hsetnx，不存在则插入，存在则失败<br>
3. hincrby，哈希表中某个value + n，只适合处理数字类型的value<br>
4. hincrbyfloat，哈希表中某个值增加，可以是小数
## 使用哈希存储的场景
1. 当作结构体/对象使用，存储一个具有多个属性的对象，可以节省分开存的key的个数，适合存结构化的数据，而且只需要
改一个字段，不需要将整个字段读出来再写回去
2. 与数据库相比，使用哈希存储具有稀疏性，不需要的字段就不存；但是对于数据库而言，在一个表中，即使某个对象不具有某个值也需要用null来占位

# 列表
列表类似于顺序表的使用，实现容器类似于deque，提升插入、删除的效率
## lpush lpop rpush rpop
push_front pop_front push_back pop_back
## lrem lindex lrange
1. lrem key count value，表示将key列表中的count个value（有多少删多少）删除并返回
2. lrange key start stop，表示将key列表中[start, stop]的下标的内容返回（显示查询出结果的时候，前面带的数字不是下标，是结果集的序号），当下标超出的时候会取余调整
3. lindex key index，表示获得key列表中下标为index的元素
## lpushx 和 rpushx
1. x代表exist，如果列表存在的话插入，否则插入失败
## lpop 和 rpop
1. lpop keyname [count]，从左端删除，后面的count在低版本的不支持<br>
2. rpop keyname [count]，从右端删除<br>
3. 与lpush和rpush搭配就可以当作栈、队列使用
## linsert
linsert keyname [before|after] pivot value, pivot是从左到右寻找的一个值(如果没有找到就插入失败)
## lrem
lrem keyname count element，count > 0时，从左到右移除count个元素；count == 0，移除列表中所有element相等的元素；count < 0时，从右向左移除count个元素
## ltrim 和 lset
1. ltrim，ltrim keyname start stop 截取[start, stop]的区间之后，剩下部分丢弃
2. lset，lset keyname index element 将index下标设置为element，其中index超过范围时，会报错
## blpop 和 brpop
1. 两者都是阻塞等待，但是在redis中，首先第一点，这两个阻塞是特殊的，并不会影响redis核心逻辑的工作，第二点就是这个阻塞队列并不会考虑队列满的情况，只会考虑队列为空的阻塞<br>
2. blpop keyname timeout，0表示一直阻塞等待，以秒为单位等待<br>
3. 两者可以同时等待多个队列，从左到右第一个有数据出队列时返回