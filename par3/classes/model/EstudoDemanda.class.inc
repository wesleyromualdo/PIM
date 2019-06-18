<?php
class Par3_Model_EstudoDemanda extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.unidade_medida';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'uniid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array();

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'uniid' => null,
        'unidsc' => null,
        'unistatus' => null,
        'unisituacao' => null,
        'unidtinclusao' => null,
        'unicpfinclusao' => null,
        'unidtinativacao' => null,
        'unicpfinativacao' => null,
    );

    public function antesSalvar()
    {
        return parent::antesSalvar();
    }

    public function retornaPerguntasRelacionadas($perid)
    {
      if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
        	$arrPerguntasRelacionadas =
               array(
                   '645' => array('641'=>'10', '642'=>'16', '643'=>'20', '644'=>'24'), //1.4 construção infantil
                   '655' => array('654'=>'10', '653'=>'16', '652'=>'20', '651'=>'24'), //2.4 construção infantil
                   '718' => array('715'=>'36', '716'=>'36', '717'=>'36'), //1.4 construção Médio
                   '730' => array('729'=>'36', '728'=>'36', '727'=>'36'), //2.4 construção Médio
                   '680' => array('679'=>'36', '678'=>'36', '677'=>'36', '676'=>'36', '675'=>'36', '674'=>'36', '673'=>'36', '672'=>'36', '671'=>'36'), //1.4 construção Fundamental
                   '698' => array('697'=>'36', '696'=>'36', '695'=>'36', '694'=>'36', '693'=>'36', '692'=>'36', '691'=>'36', '690'=>'36', '689'=>'36'), //2.4 construção Fundamental

                   '542' => array('541'=>'10', '540'=>'16', '539'=>'20', '538'=>'24'), //1.4 Ampliação infantil
                   '552' => array('551'=>'10', '550'=>'16', '549'=>'20', '548'=>'24'), //2.4 Ampliação infantil
                   '615' => array('614'=>'36', '613'=>'36', '612'=>'36'), //1.4 Ampliação Médio
                   '627' => array('626'=>'36', '625'=>'36', '624'=>'36'), //2.4 Ampliação Médio
                   '577' => array('576'=>'36', '575'=>'36', '574'=>'36', '573'=>'36', '572'=>'36', '571'=>'36', '570'=>'36', '569'=>'36', '568'=>'36'), //1.4 Ampliação Fundamental
                   '595' => array('594'=>'36', '593'=>'36', '592'=>'36', '591'=>'36', '590'=>'36', '589'=>'36', '588'=>'36', '587'=>'36', '586'=>'36'), //2.4 Ampliação Fundamental
               );
        }else{
          $arrPerguntasRelacionadas =
               array(
                   '5903' => array('5899'=>'10', '5900'=>'16', '5901'=>'20', '5902'=>'24'), //1.4 construção infantil
                   '5913' => array('5909'=>'10', '5910'=>'16', '5911'=>'20', '5912'=>'24'), //2.4 construção infantil
                   '5925' => array('5922'=>'36', '5923'=>'36', '5924'=>'36'), //1.4 construção Médio
                   '5937' => array('5934'=>'36', '5935'=>'36', '5936'=>'36'), //2.4 construção Médio
                   '5874' => array('5865'=>'36', '5866'=>'36', '5867'=>'36', '5868'=>'36', '5869'=>'36', '5870'=>'36', '5871'=>'36', '5872'=>'36', '5873'=>'36'), //1.4 construção Fundamental
                   '5892' => array('5883'=>'36', '5884'=>'36', '5885'=>'36', '5886'=>'36', '5887'=>'36', '5888'=>'36', '5889'=>'36', '5890'=>'36', '5891'=>'36'), //2.4 construção Fundamental

                   '5807' => array('5803'=>'10', '5804'=>'16', '5805'=>'20', '5806'=>'24'), //1.4 Ampliação infantil
                   '5817' => array('5813'=>'10', '5814'=>'16', '5815'=>'20', '5816'=>'24'), //2.4 Ampliação infantil
                   '5836' => array('5833'=>'36', '5834'=>'36', '5835'=>'36'), //1.4 Ampliação Médio
                   '5848' => array('5845'=>'36', '5846'=>'36', '5847'=>'36'), //2.4 Ampliação Médio
                   '5771' => array('5762'=>'36', '5763'=>'36', '5764'=>'36', '5765'=>'36', '5766'=>'36', '5767'=>'36', '5768'=>'36', '5769'=>'36', '5770'=>'36'), //1.4 Ampliação Fundamental
                   '5789' => array('5780'=>'36', '5781'=>'36', '5782'=>'36', '5783'=>'36', '5784'=>'36', '5785'=>'36', '5786'=>'36', '5787'=>'36', '5788'=>'36'), //2.4 Ampliação Fundamental
               );
        }

        return $arrPerguntasRelacionadas[$perid];
    }

    public function pegaRespostaPerguntaTexto($per, $qrpid)
    {
    	$sql = "SELECT resdsc FROM questionario.resposta WHERE perid = $per AND qrpid = $qrpid";

    	return $this->pegaUm($sql);
    }

    public function buscaEstimativaAlunos($perid, $qrpid)
    {
        $arrPerguntas = $this->retornaPerguntasRelacionadas($perid);

        $qtd = 0;
        foreach($arrPerguntas as $per => $val){
            $valor  = $this->pegaRespostaPerguntaTexto($per, $qrpid);
            $qtd    = $qtd+($valor*$val);
        }

        return $qtd;
    }

}
