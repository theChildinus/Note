# Fabric-ca

Fabric CA 是 Hyperledger Fabric 的证书颁发机构

## Fabric-ca-server

初始化 `Fabirc-ca-server`：`fabric-ca-server init -b admin:adminpw`

启动 `Fabric-ca-server` 服务: `fabric-ca-server start -b admin:adminpw`

```txt
bin
├── ca-cert.pem
├── fabric-ca-server-config.yaml
├── fabric-ca-server.db
├── IssuerPublicKey
├── IssuerRevocationPublicKey
└── msp
    └── keystore
        ├── 980917296a7fbc47e2cbccbeefc32294d80d0db66991a40ff24157c28f98820b_sk
        ├── IssuerRevocationPrivateKey
        └── IssuerSecretKey

```

## Fabric-ca-client

### 生成管理员凭证

- 指定管理员路径：`export FABRIC_CA_CLIENT_HOME=/opt/app/fabric-ca/clients/admin`
- `mkdir -p $FABRIC_CA_CLIENT_HOME`
- 管理员登录：`fabric-ca-client enroll -u http://admin:adminpw@localhost:7054`

```txt
admin
├── fabric-ca-client-config.yaml
└── msp
    ├── cacerts
    │   └── localhost-7054.pem
    ├── IssuerPublicKey
    ├── IssuerRevocationPublicKey
    ├── keystore
    │   └── 84f61e1f029343f8182dacba7718a9cfbc7c23c301689e3c31680428740f46d2_sk
    ├── signcerts
    │   └── cert.pem
    └── user
```

### 查看联盟成员

需要将当前的客户端目录 `FABRIC_CA_CLIENT_HOME` 切换到管理员目录上，然后 `fabric-ca-client affiliation list` 查看联盟成员

```txt
affiliation: .
   affiliation: org1
      affiliation: org1.department1
      affiliation: org1.department2
   affiliation: org2
      affiliation: org2.department1
```

### 注册新用户

在管理员目录下，执行命令：

指定要使用的用户的client目录：

`export FABRIC_CA_CLIENT_HOME=/home/kong/goProject/src/github.com/hyperledger/fabric-ca/bin/clients/admin`

注册新用户

`fabric-ca-client register --id.name admin2 --id.secret admin2pw --id.affiliation org1.department1 --id.attrs 'hf.Revoker=true,admin=true:ecert'`

该命令创建了用户 `admin2`，密码为 `admin2pw`，组织为 `org1.department1`，默认类型为 `user`，还有其他两种类型 `peer`，`app`

新用户注册后，会自动为新用户生成一个密码，新用户需要使用这个密码，生成自己的凭证，就像第一个用户那样。

为新注册的用户 admin2 准备一个新目录

- `export FABRIC_CA_CLIENT_HOME=/home/kong/goProject/src/github.com/hyperledger/fabric-ca/bin/clients/admin2`
- `mkdir -p $FABRIC_CA_CLIENT_HOME`

然后生成凭证：

`fabric-ca-client enroll -u http://admin2:admin2pw@localhost:7054`，admin2用户文件夹布局如下：

```txt
admin2
├── fabric-ca-client-config.yaml
└── msp
    ├── cacerts
    │   └── localhost-7054.pem
    ├── IssuerPublicKey
    ├── IssuerRevocationPublicKey
    ├── keystore
    │   └── 996d4d17553212b29a606d286995ed1af84278883c61e01d1e00946c67149ec1_sk
    ├── signcerts
    │   └── cert.pem
    └── user
```

### 查看用户详情

在管理员目录下，执行命令：

`fabric-ca-client identity list`

```txt
// 管理员
Name: admin, Type: client, Affiliation: , Max Enrollments: -1, Attributes: [{Name:hf.Registrar.Roles Value:* ECert:false} {Name:hf.Registrar.DelegateRoles Value:* ECert:false} {Name:hf.Revoker Value:1 ECert:false} {Name:hf.IntermediateCA Value:1 ECert:false} {Name:hf.GenCRL Value:1 ECert:false} {Name:hf.Registrar.Attributes Value:* ECert:false} {Name:hf.AffiliationMgr Value:1 ECert:false}]

// 用户 user1
Name: user1, Type: client, Affiliation: org1.department1, Max Enrollments: -1, Attributes: [{Name:hf.EnrollmentID Value:user1 ECert:true} {Name:hf.Type Value:client ECert:true} {Name:hf.Affiliation Value:org1.department1 ECert:true}]

// 用户 admin2
Name: admin2, Type: client, Affiliation: org1.department1, Max Enrollments: -1, Attributes: [{Name:hf.Revoker Value:true ECert:false} {Name:admin Value:true ECert:true} {Name:hf.EnrollmentID Value:admin2 ECert:true} {Name:hf.Type Value:client ECert:true} {Name:hf.Affiliation Value:org1.department1 ECert:true}]
```

默认情况下是不允许删除用户的，可以修改 server 属性允许删除

### 基于属性的访问控制

访问控制决策可以由基于身份属性的链码（和由Hyperledger Fabric运行时）来实现，称为基于属性的访问控制，简称ABAC。

为了使这成为可能，身份的注册证书（ECert）可以包含一个或多个属性名称和值。然后链码提取属性值来进行访问控制决策。

有两种方法获得具有属性的注册证书：

- 注册身份时，可以指定为身份颁发的注册证书应默认包含的属性。此行为可以在注册时重写，但这对于建立默认行为非常有用，如果注册发生在您的应用程序之外，则不需要任何应用程序更改。
- 登记身份时，可以显式请求将一个或多个属性添加到证书中。对于所请求的每个属性，可以指定属性是否是可选的。如果不需要请求，并且身份不具有属性，则会发生错误。

| 属性名 | 类型 | 描述 |
| --- | --- | --- |
| hf.Registrar.Roles | List | 注册者可以管理的角色列表 |
| hf.Registrar.DelegateRoles | List | 注册者可以授予被注册身份hf.Registrar.Roles属性的角色列表 |
| hf.Registrar.Attributes | List | 注册者允许注册的属性列表 |
| hf.GenCRL | Boolean | 如果 true 则身份可以生成CRL |
| hf.Revoker | Boolean | 如果 true 则身份可以撤销用户或证书 |
| hf.AffiliationMgr | Boolean | 如果 true 则身份可以管理组织关系 |
| hf.IntermediateCA | Boolean | 如果 true 则身份可以登记中间CA Server |

## MSP

![fabric](image/fabric.png)

### 什么是MSP？

MSP是Membership Service Provider的缩写，直译为成员关系服务提供者。为了更好的理解MSP，这里首先介绍MSP中的要用到的一些术语。

#### Certificate 证书

证书（certificate）是Fabric中权限管理的基础。目前采用了基于ECDSA算法的非对称加密算法来生成公钥和私钥，证书格式则采用了X.509的标准规范。

Fabric中采用单独的Fabric CA项目来管理证书的生成。每一个实体、组织都可以拥有自己的身份证书，并且证书也遵循了组织结构，方便基于组织实现灵活的权限管理。

#### Member 成员

成员提供节点服务，是拥有**网络唯一根证书**的合法独立实体。在Fabric区块链中，peer节点和app client这样的网络组件实际上就是一个Member

> Fabric 中的 channel 对应一个联盟，一个通道上可以由多个组织，每个组织 (org) 有多个成员

#### Organization 组织

组织（organization）代表一组拥有共同信任的根证书（可以为根CA证书或中间CA证书）的成员。这些成员由于共享同样的信任根，彼此之间信任度很高，可以相互交换比较敏感的内容。

同一个组织的成员节点在网络中可以被认为是同一个身份，代表组织进行签名。组织中成员可以为普通成员角色或者管理员角色，后者拥有更高的权限，可以对组织配置进行修改。

#### Consortium 联盟

联盟由若干组织构成的集合，是联盟链场景所独有的结构形式。联盟一般用于多个组织相互合作的场景，例如某联盟中指定需要所有参与方同时对交易背书，才允许在网络中进行执行。

联盟中的组织成员会使用同一个排序服务，并且遵循相同的通道创建策略

#### MSP 成员关系服务提供者

成员服务提供者（MSP）是一个提供抽象化成员操作框架的组件。

MSP将颁发与校验证书，以及用户认证背后的所有密码学机制与协议都抽象了出来。

一个MSP可以自己定义身份，以及身份的管理（身份验证）与认证（生成与验证签名）规则。也就是说，在一个运行的fabric系统网络中有众多的参与者，MSP就是为了管理这些参与者，可以辨识验证哪些人有资格，哪些人没资格，既维护某一个参与者的权限，也维护参与者之间的关系。

#### MS 成员服务

成员服务在许可的区块链网络上认证、授权和管理身份。在peer和order中运行的成员服务的代码都会认证和授权区块链操作。它是基于PKI的MSP实现。
