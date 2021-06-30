[toc]



### 一：类型

    1. 值类型：整型，浮点型，布尔型，枚举型等
    2. 引用类型：类，接口，数组，委托，字符串等
    
    1.1 整形：byte short int long，且有和无符号
         sbyte byte
         short ushort
          int uint
          long ulong
    1.2 浮点型：float double decimal
    1.3 字符型：占两个字节 
    char ch = 'a';
    1.4 字符串型：引用类型
    string str = "abcdedge";
    1.5 布尔类型：只有true 和 false
    bool b = true;
    C#7 中添加二进制字面值和数字分隔符 0b100_000 3.14_15926
    与python中类似的''' 保存字符串格式的是@    
    
    定义变量时可以指定引用变量，ref int value = IntValue
    定义函数时也可以指定引用返回值，可以返回一个引用类型的变量
    也就是说： arrays 这种本身就是引用类型的变量只需在return ref xxname[index]这样，而int这种要求 value 本身用ref修饰。如下例子
         class Test2
        {
            static ref int RefReturn(ref int value) { value *= 2; return ref value; }
            static ref int RefReturn() { int[] arrays = { 1,2,3}; return ref arrays[0]; }
        }

### 二：类

类的访问属性 类的修饰符 类名
    {
        类的成员
    }
    类的访问属性：public internal（不写）表示只能在当前项目中访问类
    类的修饰符：abstract 抽象，不能被实例化
               sealed   密封，不能被继承
               static   静态，不能非实例化

    类成员包括：字段 属性 函数
    类成员的访问修饰符：public：成员可以被任何代码访问
                      private：成员仅能被当前类中代码访问，为默认
                      internal：成员仅能被当前项目访问
                      protected：成员只能由当前和派生类代码访问
                      后两个可以组合使用，表示只能由当前项目的派生类访问
    
    类中的字段：
    定义：访问修饰符 修饰符 字段名 = 字段初始值（可省略）；
    修饰符：readonly  只读 表示只能由构造函数过程中或者初始化语句赋值
           static    类静态
    
    类中的函数：
    定义：访问修饰符 修饰符  返回值类型 函数名(参数列表){}
    函数修饰符
     virtual 方法可以重写
     abstract 方法必须在非抽象类的派生类中重写
     override 重写了一个基类方法，如果是重写基类方法，必须指定
     sealed  如果使用override重写了基类方法，那么可以同时指定sealed禁止派生类再重写
     static 
     extern 函数定义放在其他地方
    在一个函数的内部可以定义另一个函数，称为局部函数，仅能在当前函数中调用


    类中的属性：
    定义方式：public 数据类型 属性名{ get{ 获取属性的语句块；return 值；} set{设置属性的语句块；（xxx = value）}}
    属性一般和字段联合使用，如字段名是name，属性名应是Name，在get和set访问器中对应设置或读取
    属性可以使用virtual override abstract 关键字，但字段不行。get和set 访问器可以设置自己的访问权限
    C#6 引入了“基于表达式的属性” 如下：
    private int myDoubleInt = 4;
    public int MyDoubleIntProp => (myDoubleInt * 2);
    
    属性与字段的应用
    1. 一般情况下，在访问状态时最好提供属性而不是字段，因为这样可以更好的控制行为
    2. 对属性的读写访问可以由对象明确定义，可以设置属性是只读还是只写的或者都行
    3. 还可以为字段和属性设置可访问性，一般将字段设置为私有，通过公共属性访问字段，这样类中的代码就可以直接使用字段的数据，而公共属性可以禁止用户设置无效的数据
    
    类的构造与析构
    1. 类只有一种初始化方式就是new，可以通过限制构造函数的访问权限来禁止某一种参数的初始化方式
    2. 无法主动调用类的析构函数，一般并不提供析构函数，除非你需要在析构函数中做一点事情
    3. 为了初始化类的静态成员，可以提供一个静态构造函数，仅能有一个，不能有访问修饰符，也不能带任何参数（因为不是主动调用的），无论创建多少个类实例，静态构造仅调用一次
    3.1 创建包含静态构造函数的类对象时
    3.2 访问包含静态构造函数的类的静态成员（包括方法，属性，字段）时
    4. 可以定义一个静态类，这个类中仅能包含静态成员。定义方式：访问修饰符 static class ClassName{}
    
    类函数的重载：参数个数及类型不同(引用可构成重载)
    类函数的参数：除了基础的普通参数外，还可以有引用参数和输出参数，引用ref定义，输出out定义
    				 调用带引用参数的函数时，实际参数必须是一个变量，并且调用时指定ref关键字
    				 输出参数多用于一个函数需要返回多个值的情况，在函数返回之前记得为输出参数赋值，调用函数时指定out关键字
    可以为函数指定一个特殊参数，必须是最后一个参数，称为参数数组，参数数组允许不定个数的参数来调用，是为了简化调用时必须创建一个数组，使用params关键字来指示
    out 与 ref的区别：
    1. 把未赋值的变量用作ref是非法的，用作out是合法的
    2. 函数内部使用out参数时，必须看作尚未赋值

   // 委托：是一种存储函数引用的类型
    // 委托的声明指定了一个返回类型和参数列表。
    // 定义了委托之后就可以定义该类型的变量，可以初始化为具有相同返回类型和参数的函数引用，之后可以使用委托变量来调用这个函数（类似c中的函数指针）
    // 定义方式 delegate double ProcessDelegate(double param1, double param2);
    // 使用方式：ProcessDelegate  process  = new ProcessDelegate(FunName);ProcessDelegate process = FunName;


    // 元组：函数返回多个值有很多方法，如用结构，类，数组，out参数，但比较繁琐，使用元组是一种比较优雅的方法。
    // 定义一：var numbers = (1, 2, 3, 4, 5);
    // 使用时 number.Item1
    // 定义二：(int one, int two, int three, int four) numbers = (1,2,3,4);
    // 使用时：number.one
    // 示例： public static (int max, int min, double value) GetMaxMin(params double[] arrays) {return (1,2,3); }


​    

    // lambda表达式
    // 定义方式：访问修饰符  修饰符 返回值类型  函数名(参数列表)  => 表达式；
    // 			  public  static int Add(int a, int b) => a + b;
    // 注意：在函数中表达式只有一条语句的情况下，如果函数定义了返回值类型，在表达式中不必使用return关键字
    // 
    
    // 递归
    // 嵌套类
    // 部分类:一个类可以由多个部分类组成，定义部分类的语法是  访问修饰符 修饰符 partial class 类名{....}
    // 部分函数：在一个部分中定义函数，partial 返回值类型 函数名()； 在另一部分中定义函数体。必须私有，不能由返回值，不能使用out类型参数
    
    // Console类
    // 常用函数：WriteLine Write Read ReadLine
    // Write(格式化字符串, 输出项1, 输出项2), 格式化字符串中使用{0}索引方式。
    
    // Math类
    // 常用函数：Abs 取绝对值 Ceiling Floor Equals Max Min Sqrt Round
    
    // Random类
    // 构造：new Random()使用当前时间作为随机数种子 New Random(Int32)使用指定值作为随机数种子，一般是
    // 函数：Next Next(int max) Next(int min, int max) NextDouble()  NextBytes(byte[] buffer)
    
    // DateTime类
    // 静态函数：now 获取当前日期和时间
    // 常用函数: Date Day DayOfWeek DayOfYear Add AddDays AddHours  AddMinutes AddSeconds AddMonths AddYears
    // 两个日期的差可以由TimeSpan表示，TimeSpan ts = dt2 - dt1； ts.Days;   
    
    // string
    // 常用函数和属性：length IndexOf LastIndexOf StartsWith EndsWith ToLower ToUpper Trim Remove TrimStart TrimEnd PadLeft PadRight Split Replace SubString Insert Concat
    // 类型转换： 变量类型.Parse(字符串类型的值);
    // 			  convert.To数据类型(变量名);
    // 			  变量.ToString();
    
    // 装箱和拆箱
    // 装箱：将值类型转换为引用类型的操作称为装箱
    // 拆箱：将引用类型转换为值类型的操作称为拆箱
    
    // 正则表达式:主要作用是验证字符串的值是否满足一定的规则，在页面输入数据验证方面的应用比较多。
    //           主要分为元字符和表示重复的字符
    // 元字符：. 匹配除换行符以外所有的字符
    //        \w 匹配字母数字下划线 
    //        \s 匹配空白符（空格）
    //        \d 匹配数字
    //        \b 匹配表达式的开始或结束
    //        ^ 匹配表达式的开始
    //        $ 匹配表达式的结束
    // 表示重复的字符：* 0次或多次字符
    //               ? 0次或1次字符
    //               + 1次或多次字符
    //               {n} n次字符
    //               {n,M} n到M次字符
    //               {n,} n次以上字符
    // 正则表达式可由 | 表示多个正则表达式之间的关系：如对身份证的验证一代15字符二代18个字符 \d{15} | \d{18}
    // Regex 类在System.Text.RegularExpressions 名称空间中
    
    // 结构
    // 结构是值类型
    // 结构中可以包含变量和函数，添加合适的函数可以处理常见的事务
    struct Direct
    {
        public orientation direct;
        public double distance;
    }
    struct PersonName
    {
        string firstName;
        string LastName;
        public string Name() => firstName + " " + LastName;
    }

- 接口

  - 接口的定义：

  ```C#
  
  public interface IDisposable{
          void Dispose();
  }
  ```

  - 接口是把公共实例方法和属性组合起来，以封装特定功能的集合。

  - 可删除的对象

    - IDisposable接口，支持IDisposable接口的对象必须实现Dispose方法，当不再需要某个对象时，就调用这个方法，释放重要资源，否则，等到垃圾回收时才会释放重要资源。
    - C#允许以一种优化的方式使用这个方法。

    ```c#
    ClassName VarName = new ClassName();
    using (<VarName>){ }
    或者
    using (ClassName VarName = new ClassName()){....}
    ```

- 继承

  - 基类的成员可以是虚拟的，派生类就可以重写。虚拟成员不能是私有成员，因为这样会自相矛盾，不能既要求派生类重写成员，又不让派生类访问成员
  - 派生类会继承其基类的接口。

- 多态

  - 可以把某个派生类型的变量赋给基本类型的变量

  - 接口的多态性：

    - 尽管不能像对象那样实例化接口，但是可以建立接口类型的变量。然后在支持该接口的对象上，使用这个变量来访问该接口提供的方法和属性。

    - ```c#
      IConsume consumeInterface;
      consumeInterface = myCow;
      consumeInterface.EatFood();
      consumeInterface = myChicken;
      consumeInterface.EatFood();
      ```

- 对象之间的关系

  - 包含
  - 集合
    - 集合基本上就是增加了功能的数组，Add 和Remove和Item

- 运算符重载

- 事件

- 引用类型和值类型

  - string object 数组也是隐式的引用类型，我们创建的每个类也是引用类型。
  - 构造函数初始化器：类默认构造可以在括号之后添加 `： this(4，5)`调用其它构造函数

- 接口和抽象类的异同

  - 相同之处
    - 抽象类和接口都包含可以由派生类继承的成员。
    - 抽象类和接口都不能直接实例化，但可以声明这些类型的变量，以便使用继承它们对象的多态性
  - 区别
    - 派生类只能继承一个基类，即只能直接继承一个抽象类，相反，类可以继承多个接口
    - 抽象类可以拥有抽象成员（没有代码体，且必须在非抽象派生类中实现）和非抽象成员（它们拥有代码体，也可以是虚拟的，这样在派生类中可以重写），另一方面，接口成员必须实现
    - 接口的成员都是公共的，但抽象类的成员可以是私有的（只要它们不是抽象的），受保护的，内部的或受保护的内部成员
    - 接口不能包含字段，构造析构函数，静态成员或常量
  - 注意：
    - 抽象类主要用作对象系列的基类，这些对象共享某些主要的特性。接口则主要用于类，这些类存在根本性区别，但仍可以完成某些相同任务

- 浅度和深度复制

- 重构字段

  - 对字段右键快速重构，接受默认选项，就会默认将字段变为私有，并且增加一个属性

- 自动属性：上面的重构字段还是要编写字段的代码，这时可以直接定义属性，编译器内部会自动声明存储属性的私有字段

- 类成员的其他主题

  - 隐藏基类方法
    - 如果派生类重新定义了与基类相同函数签名的函数，没有使用关键字new的话，编译器会产生一个警告，提示基类成员被隐藏
  - 调用重写或隐藏的基类方法
    - 使用base或this关键字，在属性声明中使用this关键字能明显看出使用的是类成员而不是局部变量。public int SomeData => this.someData
  - 使用嵌套的类型定义：嵌套类可以访问被嵌套类的私有成员，嵌套类可以提供方法，在方法内就可以访问修改私有成员
    - 将类的内部状态提供给内部的嵌套类在某些情况下十分有用，但通常情况下通过类提供的方法操作内部状态就足够了。

- 接口的实现

  - 要隐藏继承自接口的成员，可以用关键字new来定义它们

  - 实现接口的类必须包含该接口所有成员的实现代码，且必须匹配指定的签名（包括匹配指定的get和set块），且必须是公共的

  - 可使用关键字virtual 和 abstract 来实现接口成员，但不能使用const或static

  - 可以在基类中隐式实现接口成员。（子类继承一个基类和一个接口，基类中有和接口同签名的成员即隐式实现）

  - 在基类中将接口实现代码定义为virtual十分有用，这样子类就可以重写该实现代码

  - 显示实现接口成员：只能通过接口来访问该成员。

    - ```c#
      public class MyClass : IInterface
      {
          void IInterface.DoSomething(){}
      }
      IInterface myInt = new MyClass(); myInt.DoSomething;
      ```

  - 如果实现带属性的接口，就必须实现匹配的访问器。这句话并不是完全正确，如果接口定义的属性只有set，那么可以添加get，反之亦然。但只有隐式实现接口才能这么做。

### 三：集合、比较和转换

#### 3.1 集合

1. 使用数组创建包含一组对象或值的变量类型，但数组是有限制的，最大得限制是数组一旦创建大小是不可更改的。c#中的数组是System.Collection的实例。是集合类的一种类型。

   1. IEnumerable：可以迭代集合中的项
   2. ICollection：继承于IEnumerable。可以获取集合中项的个数，并能把项复制到一个简单的数组类型中。
   3. IList：继承于IEnumerable和ICollection。提供了集合的项列表，允许访问这些项，并提供其他一些与项列表相关的基本功能。
   4. IDictionary：继承于IEnumerable和ICollection，类似于IList，但提供了可通过键值访问的项列表

2. 使用集合

   1. `System.Collections.ArrayList.Add.AddRange.Remove.RemoveAt`

3. 定义集合：从System.Collections.CollectionBase派生自己的**强类型**集合

   1. CollectionBase类有接口IEnumerable，IList，ICollection，但只提供了一些必要的代码。主要是IList的Clear和RemoveAt以及ICollection的Count属性
   2. 为便于完成任务，CollectionBase提供了2个受保护的属性，它们可以访问存储的对象的本身，可以使用List和InnerList

   ```c#
               Animals animals = new Animals();
               animals.Add(new Cow("cow1"));
               animals.Add(new Chicken("chicken1"));
               foreach(Animal animal in animals)
               {
                   Console.WriteLine($"{animal.Name}");
               }
   ```

4. 使用索引符

   ```c#
       public class Animals : CollectionBase
       {
           public void Add(Animal animal) => List.Add(animal);
           public void Remove(Animal animal) => List.Remove(animal);
   
           public Animals() { }
   
           public Animal this[int index]
           {
               get { return (Animal)List[index]; }
               set { List[index] = value; }
           }
       }
   ```

5. 链控集合和IDictionary

   1. 实现Dictionary接口允许使用键值来进行索引。这个基类是DictionaryBase

   ```c#
       public class AnimalDictionary : DictionaryBase
       {
           public void Add(string id, Animal animal) => Dictionary.Add(id, animal);
   
           public void Remove(string id) => Dictionary.Remove(id);
   
           public AnimalDictionary() { }
   
           public Animal this[string id]
           {
               get { return (Animal)Dictionary[id]; }
               set { Dictionary[id] = value; }
           }
       }
               AnimalDictionary animals = new AnimalDictionary();
               animals.Add("1", new Cow("cow1"));
               animals.Add("2", new Chicken("chicken1"));
               
               foreach(DictionaryEntry entry in animals)
               {
                   Console.WriteLine($"{entry.Value.ToString()}  {((Animal)entry.Value).Name}");
               }
   ```

6. 迭代器：IEnumerable 一般用来迭代类成员，IEnumerator 一般用来迭代类，在迭代器块中使用yield来选择要在foreach循环中使用的值

```c#
        public IEnumerable Ages
        {
            get
            {
                foreach (DictionaryEntry p in Dictionary)
                {
                    Person person = p.Value as Person;
                    yield return person.Age;
                }
            }
        }

        public new IEnumerator GetEnumerator()
        {
            foreach(DictionaryEntry p in Dictionary)
            {
                Person person = p.Value as Person;
                yield return person.Age;
            }
        }
```

7. 迭代器和集合

8. 深度复制
   1. 实现ICloneable接口，该接口有一个clone方法
   2. 在比较复杂的类中，clone是一个递归的过程

```C#
public class Cloner : IConeable{
    public Content MyContent = new Content(11);
    public Object Clone(){
        Cloner clonedCloner = new Cloner();
        clonedCloner.MyContent = MyContent.Clone();
        return clonedCloner;
    }
}
```

#### 3.2 比较

1. 类型比较

   1. GetType和typeof运算符一起使用可以在运行时判断某个对象是否是某个类型。

2. 封箱和拆箱

   1. 封箱将值类型转换为System.Object类型，或者转换为由值类型实现的接口类型。拆箱是相反的转换过程 
   2. 封箱和拆箱后操作的是值类型的一个副本引用，不会改变被封箱的值类型变量的值，封箱是隐式的，拆箱需要明确转换的类型

3. is运算符

   1. is运算符用来检查对象是不是给定类型。或者是否可以转换为给定类型
   2. 使用GetType和typeof只能判断是一种类型，而不能判断是否可以转换成给定类型
   3. is语法：operand is type
      1. 如果type是一个类类型，而operand也是该类型，或者它继承了该类型，或者它可以封箱到该类型，返回true
      2. 如果type是一个接口类型，而operand也是该类型，或者它是实现接口的类型，则结果为true
      3. 如果type是一个值类型，而operand也是该类型，或者它可以拆箱到该类型，返回true

4. 值比较

   1. 可使用运算符重载，使用IComparable和IComparer接口，.net framework中的各种集合类支持这种方式，通常运用于集合中对对象的排序

   2. 运算符重载

      1. 要重载运算符，可给类添加运算符类型成员，必须是static，并且要指定处理的操作数

      ```c#
      public class AddClass1{
          public int Val;
          public static AddClass1 operator +(AddClass1 op1, AddClass1 op2){
      		return new AddClass1(op1.Val + op2.Val);
          }
          public static AddClass1 operator -(AddClass1 op1){
              return new AddClass1(-op1.Val);
          }
      }
      ```

   3. 可以重载的运算符：

      1. 一元：`+ -  ~ ! ++ -- true false`
      2. 二元：`+ - * / % & | ^ << >`
      3. 比较：`== != < > <= >=`

   4. 一些意义相关的运算符可以成对重载，以便减少代码量。例如>= 和 < ，它们的关系是相反的。

      1. 同样适用于 == 和 != ，对于这两个运算符，一般还需重载Object.Equals和Object.GetHashCode，因为这两个函数一般也用来比较对象后面的两个函数不是运算符重载，而是虚函数重写，需要override而不是static

5. IComparable 和 IComparer接口

   1. 区别

      1. IComparable 在要比较的对象的类中实现，可以比较该对象和另一个对象

      ```c#
      if(person1.CompareTo(person2) > 0){...}
      ```

      1. IComparer在一个单独的类中实现，可以比较任意两个对象

      ```c#
      if(PersonComparer.Comparer(person1, person2) > 0){ ...}
      ```

      1. 一般使用IComparable 给出类的默认比较代码，使用其他类给出非默认的比较代码

   2. Net Framework 在类 Comparer 上提供了ICompaer接口的默认实现代码。
   
   ```c#
   Comparer.Default.compare(firstString, secondString);
   ```
   
6. 对集合排序

   1. 许多集合类可用对象默认的方式进行排序，或用定制方式进行排序，比如ArrayList.sort方法，可以传递一个ICompare接口。

   ```c#
   public class Person : IComparable{
       public string Name;
       public int Age;
       public int CompareTo(Object other){
           if(other is Person ){
               Person otherPerson = other as Person;
               return this.Age - otherPerson.Age;
           }
           else {
               throw new ArgumentException("Object to compare to is not a Person");
           }
       }
   }
   public class PersonCompareName : IComparer{
       public static IComparer Default = new PersonCompareName();
       public int Compare(Object x, Object y){
           if(x is Person && y is Person){
               return Compare.Default.Compare((Person)x.Name, (Person)y.Name);
           }
           else{
               throw new ArgumentException("One of both objects to compare are not Person Objects");
           }
       }
   }
   ArrayList list = new ArrayList();
   list.add(new Person(1, "xiaoming"));
   list.add(new Person(2, "xiaohong"));
   list.sort();// 默认调用Person.CompareTo方法
   list.sort(PersonCompareName.Default);// 使用自定义的PersonCompareName比较
   ```

#### 3.3 转换

1. 重载转换运算符

   1. 可以定义类型之间的显示或隐式转换，如果要在不相关的类型之间进行转换，如没有继承关系，也没有共享接口，那么就必须重载转换运算符。
   2. 显示转换使用explicit，隐式转换使用implicit

   ```c#
   public class ConvClass1{
       public static implicit operator ConvClass2(ConvClass1 op1){
           ConvClass2 ret = new ConvClass2();
           ret.value = this.value;
           retur ret;
       }
   }
   ```

2. as运算符

   1. as将一种类型转换为指定的引用类型
   2. 语法：operand as type
      1. operand的类型是type
      2. operand可以隐式转换为type
      3. operand可以封箱到type
      4. 如果不能从operand到type，那么结果就是null
   3. 优点：使用as转换类型失败结果是null，而使用强转方式转换失败会抛出异常。

### 四：泛型

#### 4.1 使用泛型

1. 可空类型：值类型必须包含一个值，可以存在未赋值的值变量，但不能使用。而引用类型可以是null

   1. 泛型使用System.Nullable<T> 提供了使值类型为空的一种方式。例如`System.Nullable<int> = 	nullableInt`
   2. int? nullableInt; 是上述语法的缩减。
   3. 运算符和可空类型：对于一个或两个变量是null的情况下，进行四则运算的结果，除了bool? 外的所有简单可空类型，该结果为null
   4. ?? 运算符：op1 ?? op2，是为了方便可空类型的运算，如果op1不为null，那么结尾是op1否则是op2
   5. ?. 运算符：`int? count = customer.orders?.Count();`当orders为null的情况下，count是null，结合之前的??运算符，可以为count添加一个默认值`int? count = customer.orders?.Count() ?? 0`
      1. 这个运算符另一个特点是触发事件
      2. 重载==运算符时，如果没有检查null，就会触发System.NullReferenceException，之前编写的Card类就没有检查，用?.来优化一下`public static bool operator == (Card card1, Card card2) => (card1?.suit == card2?.suit)&&(card1?.rank == card2?.rank)`

2. 使用 `System.Collections.Generic`名称空间中的泛型

   1. List<T>：相当于实现了继承自CollectionBase的强类型集合，

   2. 对泛型列表进行排序和搜索

      1. 要对List<T>排序，	可以在要排序的类型上提供IComparable<T>接口，或者提供ICompaer<T>接口，另外还可以提供泛型委托作为排序方法。
      2. 要使用委托为泛型列表进行排序，要创建一个委托类提供搜索和比较的函数，然后实例化Comparsion<T>或Predicate<T>泛型类，传递给sort函数。

   3. 示例：

      ```c#
      public class Animals : List<Animal>{
          public Animals{}
          public Animals(IEnumable<Animal> initializeItems){foreach(Animal item in initializeItems{Add(item);})}
      }
      public static class AnimalDelegate{
          public static int ForCompare(Animal x, Animal y){
              return x.Age - y.Age;
          }
          public static bool ForSearch(Animal x){
              return x.Age == 666;
          }
      }
      Animals animals = new Animals();
      animals.Add(new Animal(10));
      animals.Add(new Animal(20));
      animals.Add(new Animal(666));
      Comparsion<Animal> sorter = new Comparsion<Animal>(AnimalDelegate.ForCompare);
      animals.sort(sorter);
      Predicate<Animal> searcher = new Predicate<Animal>(AnimalDelegate.ForSearch);
      Animas searchAnimals = new Animals(animals.FindAll(searcher));
      ```

   4. Dictionary<k,v>：

      1. 每个key应该是唯一的，否则抛出ArgumentException异常，所以Dictionary支持将IComparer<T>接口传递给其构造函数
      2. 如果要把自己的类用作键，且它们不支持IComparable<T>接口，或ICompaer<T>接口，或者要使用非默认的比较接口，就必须传递一个IComparer<T>接口给其构造函数。

3. 定义泛型类型

   1. 定义泛型类

      1. 使用default赋予默认值：`innerObject = default(T1)`如果T1是引用类型，那么默认值是null，如果是值类型，那么初始化为0

   2. 约束类型，限制实例化泛型类的类型由某个特性

      1. `class MyGenericClass<T> where T : Animal `：限制T是Animal及其子类

   3. 从泛型类中继承

      1. 如果一个类型继承的基类型受到了约束，那么该类型就不能“解除约束”，但可以约束为基类型约束的一个子集。

   4. 泛型运算符

      1. 定义如下泛型类的隐式转换运算符

         ```c#
         public static implicit operator List<Animal> (Farm<T> farm){
             List<Animal> result = new List<Animal>();
             foreach(T item in farm){
                 result.Add(item);
             }
             return result;
         }
         ```

   5. 泛型结构

4. 定义泛型接口

   1. 定义泛型接口与泛型类技术相同。

5. 定义泛型方法

   1. 泛型方法可以提供给非泛型类
   2. 如果类是泛型的，那么泛型方法要采用跟泛型类不同的标识符，否则不会编译。同时，可以为泛型方法的 标识符采取约束。
   3. 泛型方法的泛型类型参数（<>里面的类型参数）不同也可以构成重载。

6. 定义泛型委托

   ```c#
   // 一般委托
   public delegate int MyDelegate(int op1, int op2);
   // 泛型委托
   public delegate T1 MyDelegate<T1, T2>(T op1, T op2) where T1 : T2;	
   ```

#### 4.2 变体

1. 多态性允许将派生类的对象赋值给基类型的对象，但是不适用于接口，如下，可以通过在泛型接口，泛型委托上定义变体参数来支持这种需求。

   ```c#
   IMethandProducer<Cow> cowProducer = myCow;
   IMethandProducer<Animal> animalProducer = cowProducer; // error
   ```

2. 协变：不能把泛型接口值放在使用基类型的泛型接口变量中（如上）

3. 抗变：不能把泛型接口值放在使用基类型的泛型接口变量中（如上），但可以放在派生类型的泛型接口变量中

   ```c#
   IMethandProducer<Cow> cowProducer = myCow;
   IMethandProducer<SuperCow> superCowlProducer = cowProducer;//success
   ```

4. 协变->变体

   1. 要把泛型类型参数定义为协变，可在类型定义中使用out关键字`public interface IMethodProducer<out T>{...}`
   2. 对于接口定义，协变类型参数只能用作方法的返回值或属性get访问器
   3. 例子：用于协变枚举

5. 抗变->变体

   1. 要把泛型类型参数定义为抗变，可在类型定义中使用in关键字`public interface IMethodProducer<in T>{...}`
   2. 对于接口定义，抗变类型参数只能用作方法参数，不能用作返回类型
   3. 例子：用于抗变比较

### 五：高级C#技术

#### 5.1 自定义异常

- 继承Exception，构造函数中为base做初始化，可增加其他属性

#### 5.2 事件

- 事件由对象抛出，其他对象可订阅此事件（+=赋值操作）。

- 自定义事件

  - 首先定义一个委托类型，指定事件处理函数的返回类型和参数
  - 然后在要产生事件的类中使用event关键字，声明一个委托类型成员事件。
  - 之后就可以调用这个委托类型的成员事件来引发一个事件。

- EventHandler 和泛型 EventHandler<T>类型

  - 一般情况下，一个事件处理方法可能处理多个对象的事件，那么需要为事件处理方法添加参数来进行区别
  - 在要发出事件的类中使用`public static EventHandle<MessageArrivedArgs> MessageArived`来声明事件
  - MessageArrivedArgs类要继承自`EventArgs`
  - 发出事件时`MessageArived(this, new MessageArrivedArgd("this is message"))`
  - 在事件处理函数中即可通过source和args来进行分别处理

- 事件的返回值最好是void，因为事件是可以被多个处理方法订阅的，那么返回值会采用最后一个订阅的处理方法的返回值。并且要避免使用out类型的参数

- 匿名方法：

  - 使用匿名方法在处理事件时就不用创建一个新类了，简化了语法

  - ```c#
    myConnection.MessageArrived += delegate(Connection source, MessageArrivedEventArgs args){
        ...
    }
    ```

#### 5.3 特性

- 特性让我们可以为代码段标记一些信息，这些信息又可以从外部读取，也叫做对代码的装饰

- 读取特性的值

  - ```c#
    Type classType = typeof(SomeClass);
    object[] customAttributes = classType.GetCustomArrtibutes(true);// 第二个参数表示是否只想了解类本身的信息还是包括派生类的信息
    ```

- 创建特性

  - 通过继承自 System.Attribute ，可以创建出自己的特性。

#### 5.4 初始化器

- 语法 

  - ```c#
    ClassName varName = new ClassName{
        propertyOrField1 = xxx;
        propertyOrField2 = xxx;
    }
    ```

  - 这样的话就不必为ClassName提供一个多参数构造函数了，公开的属性字段的值可以在初始化器中直接赋值。

- 初始化器也是可以嵌套的。

- 集合初始化器（在后台，编译器为结合初始化器的每一项调用Add方法，所以集合类必须实现Add方法，否则报错）

#### 5.5 类型推理

- 依赖编译器来确定一个对象的类型

#### 5.6 匿名类型

- ```c#
  var animal = new{
      Name = "Lea",
      Age = 42,
      Weight = 11.11
  }
  ```

#### 5.7 动态查找

#### 5.8 高级方法参数

- 可选参数
  - 提供默认值：必须是字面值，常量值或者默认类型值
  - Optional定义可选参数，但不能提供默认值
- 命名参数
  - 当方法签名中包含多个可选参数时，只想为某几个参数传参，可以使用命名参数

#### 5.9 lambda 表达式

- 把lambda表达式用于匿名方法：

  - ```c#
    myTimer.Elapsed += (source, e) => Console.WriteLine("this is test for event");
    ```



### 六：c#新特性

#### C# 7.0

- 数字字面量的改进
- 输出变量及参数忽略
- 模式
  - x is string s：称为模式变量
  - switch语句同样支持，可以使用when语句指定一个判断条件，或者直接case null
- 局部方法，可以像lambda表达式一样捕获局部变量
- 更多的表达式体成员
  - c#6 引入胖箭头表达式体的方法，只读属性，运算符及索引器
  - c#7 扩展到构造函数，读/写属性，析构函数
- 解构器：Deconstruct，可以将字段反向赋值给变量，解构方法可以是扩展方法，方便对其它作者的类型进行结构
- 元组
- throw 表达式，可以出现在表达式体函数中

#### c# 6.0

- null合并运算符可以判断操作数如果不是null则结果为操作数，否则结果为默认的值：`string s = o ?? "nothing"`
- null条件运算符可以避免在调用方法或访问类型的成员之前显示的编写用于检测null的语句
- 表达式体函数可以一Lambda表达式的形式书写仅仅包含一个表达式的方法，属性、运算符、及索引器，使代码更加简短
- 属性初始化器可以对自动属性进行初始赋值，也支持只读属性
- 索引初始化器可以一次性初始化具有索引器的任意类型。
- 字符串插值可以使用美元符和花括号来解释变量
- 异常过滤器可以在catch块上使用when再加一个条件
- using static可以引入一个类型的所有静态成员
- nameof返回变量、类型、或者其它符号的名称
- 可以在catch和finally块中使用await

#### c# 5.0

- async和await，支持异步功能

#### c# 4.0

- 动态绑定
- 可选参数和命名参数
- 用泛型接口和委托实现类型变化
  - 在泛型接口和委托的类型参数上指定out修饰符可将其声明为协变参数。
    - out修饰符表明T只用于输出的位置（例如方法的返回值）
    - ![image-20210623164733864](C:\Users\killa\AppData\Roaming\Typora\typora-user-images\image-20210623164733864.png)
  - 在泛型接口和委托的类型参数上指定in修饰符可将其声明为逆变参数。
    - 使用object的default比较，用来对string比较
- 改进com互操作性

#### c# 3.0

- linq查询
- 隐式类型局部对象：var
- 匿名类型
- 对象构造器：允许在调用构造器之后，以内联的方式设置属性，从而简化对象的构造过程，支持命名类型和匿名类型
- lambda表达式：适用于创建流畅的Linq查询
- 扩展方法：可以在不修改类型定义的前提下使用新的方法扩展现有类型
- 查询表达式：提供了编写linq查询的高级语法
- 表达式树
- 自动化属性和部分方法

#### c# 2.0

- 泛型
- 可空类型：int? 实际上内部是语法糖，内部类型时Nullable<int>
- 迭代器
- 匿名方法：lambda表达式的前身



### 七：Linq查询

1. Linq支持查询任何支持了IEnumerable<T>接口的集合类型。

2. 查询运算符是Linq中进行序列转换的方法。在System.Linq命名空间的Enumerable类中定义了约40种查询运算符。这些运算符都是以静态扩展方法的形式来实现，称为标准查询运算符

3. Linq同样支持对远程数据源中动态获取的序列进行查询。这些序列需要实现IQuerable<T>接口。而在Querable类中则有一组相对应的标准查询运算符对其及进行支持。

4. 标准查询运算符

   ![image-20210623194552701](C:\Users\killa\AppData\Roaming\Typora\typora-user-images\image-20210623194552701.png)

5. 查询表达式

   ![image-20210623194723655](C:\Users\killa\AppData\Roaming\Typora\typora-user-images\image-20210623194723655.png)

6. 混合查询语法

7. 延迟执行：大部分查询运算符的一个重要特点是：它们并非在构造时执行，而是在枚举时（在枚举器上调用MoveNext）执行。

   1. 因为延迟执行从而带来一个后果就是每次在枚举linq查询的结果时都会进行一次查询，我们可以将结果通过tolist或toarray保存下来

8. 捕获变量

   1. 如果你的lambda表达式捕获了外部变量，那么该变量的值由表达式执行时决定

9. 延迟执行的工作原理

   1. 查询运算符通过返回装饰器序列来提供延迟执行的功能
   2. 每一个查询运算符都将实例化一个装饰器将上一层的序列进行包装

10. 子查询

    1. 包含在另一个查询的lambda表达式中的查询语句
    2. ![image-20210623200611558](C:\Users\killa\AppData\Roaming\Typora\typora-user-images\image-20210623200611558.png)

11. 构造复杂查询

    1. 渐进式查询构造
    2. 使用into关键字
    3. 查询的包装

12. 

