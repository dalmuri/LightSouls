# LightSouls
 
Dark Souls와 같은 게임을 모바일로 가볍게 구현해본 개인 프로젝트
- Unreal Engine 5.3 사용
- Monster Hunter Now의 조작 방식을 참고함
> ![Play1](https://github.com/dalmuri/LightSouls/assets/132140216/36f25bc1-6d6b-4eff-8758-b62340d51ee2)

## 조작
#### Tap
- 짧게 Tap하여 공격
- 여러 번 Tab하면 콤보 공격이 나감
> ![Attack](https://github.com/dalmuri/LightSouls/assets/132140216/218c1747-0443-463a-8343-b75ac98744af)

#### Swipe
- Swipe 방향으로 회피(구르기)
- 앞, 뒤, 왼쪽, 오른쪽 4방향으로 회피 가능
- 회피 중 무적시간(Invincible Frame)이 존재
- 무적시간으로 공격을 피할 시 스테미나가 소량 회복되며 초록색 이펙트가 나옴
> ![Roll](https://github.com/dalmuri/LightSouls/assets/132140216/6bdfb9b0-e3f7-4fff-b2aa-80c92d3b0c52)
> ![DodgeRoll](https://github.com/dalmuri/LightSouls/assets/132140216/ebe61ae6-ae39-498f-9007-0b9f00903278)

#### Hold
- 일정시간 이상 가만히 누르고 있으면(Hold) 방어
- 누르고 있는 동안 방어 동작을 취함
- 방어 중에는 스테미나가 회복되는 속도가 느려짐
- 방어 중 공격을 맞을 시 체력 대신 스테미나가 감소
- 스테미나가 모두 소진되면 자동으로 방어가 해제됨
> ![Guard](https://github.com/dalmuri/LightSouls/assets/132140216/b61f907e-2727-470f-9e20-397c1ce24d1e)

#### Hold + Swipe(↑)
- Hold 중 앞으로 Swipe하면 Parry
- Parry로 적의 공격을 쳐내는데 성공하면 스테미나가 상당량 회복되며 이펙트와 함께 잠시 시간이 느려지는 효과가 나옴
> ![ParryMotion](https://github.com/dalmuri/LightSouls/assets/132140216/0551807b-51e4-4eb9-b2dd-48ded1cd7810)
> ![Parry](https://github.com/dalmuri/LightSouls/assets/132140216/d1c10af3-e86c-4867-a829-94db20ce3e6c)

#### Hold + Swipe(↓←→)
- Hold 중 뒤, 왼쪽, 오른쪽으로 Swipe하면 해당 방향으로 회피(구르기)
> ![GuardRoll](https://github.com/dalmuri/LightSouls/assets/132140216/1f593fc6-ecc1-435e-99a2-7ff7a9fcc75b)

## Play
#### 피격
> ![GetHit](https://github.com/dalmuri/LightSouls/assets/132140216/ca1f08b0-211b-4b34-a16f-a560ff0dbaf4)

#### 사망
> ![Death](https://github.com/dalmuri/LightSouls/assets/132140216/3842f3a1-3b4f-4bc7-b6b6-4556f3526c67)

#### 승리
> ![Beat](https://github.com/dalmuri/LightSouls/assets/132140216/40747cab-7de0-4680-a6e1-ac06ba73d913)
