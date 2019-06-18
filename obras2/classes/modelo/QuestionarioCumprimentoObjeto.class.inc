<?php
class QuestionarioCumprimentoObjeto extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "obras2.questao_cumprimento_obj_documentacao";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("qcodid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'qcodid' => null,
        'cumid' => null,
        'qstid' => null,
        'qcodresposta' => null,
        'qcodesclarecimento' => null,
        'qcodobs' => null,
        'qcodstatus' => null,
    );

    public $codid;
    public $obrid;
    public $questoes;
    public $escopoQDCO = true;
    public $descricaoItem;
    public $escopo = 'QDCO';
    public $itcid;
    private $obra;
    public $cexid;
    public $devvalor;

    /**
     * MÃ©todo responsÃ¡vel por montar os questionÃ¡rios.
     * @param $obra ID da obra
     * @param bool $escopo Escopo (Obra principal, Obras vinculadas ou Construtoras extras)
     * @param bool $flgConstrutoraExtra Flag que indica se o questionÃ¡rio serÃ¡ de uma construtora extra.
     * @return $this
     * @tutorial O escopo 'QDCOE' indica que o questionÃ¡rio Ã© de uma construtora extra.
     */
    public function montaQuestionario($obra, $escopo = true, $flgConstrutoraExtra=false, $flgConstrutoraVinculada=false)
    {
        $this->escopoQDCO = $escopo;
        $this->codid = $obra['codid'];
        $this->obrid = $obra['obrid'];

        if($this->escopoQDCO){
       		$this->escopo = 'QDCO';
        } else {
        	if($flgConstrutoraVinculada){
				$this->escopo = 'QDCOV';
			} else {
				$this->escopo = 'QDCOC';
			}
        }
        
        $this->escopo = $flgConstrutoraExtra ? 'QDCOE' : $this->escopo;
        $this->obra = $obra;
        
        $questao = new Questao();
        $questionario = $questao->pegaTodaEstruturaCumprimentoObjeto($this->codid, $this->escopo);

        if($flgConstrutoraExtra){
            $iDConstrutoraExtra = $obra["cexid"];
        }
        $this->populaQuestionario($questionario, $flgConstrutoraExtra, $iDConstrutoraExtra);
        if ($flgConstrutoraExtra) {
            $this->populaDescricao($questionario, true);
            $this->cexid = $obra["cexid"];
        } else {
            $this->populaDescricao($questionario);
        }
        //$this->populaDescricao($questionario);
        $this->populaITCID($questionario);
        $this->recuperarGruDevolvida($this->obrid);

        return $this;
    }

    public function respostas($obra,$escopo = true, $flgConstrutoraVinculada=false)
    {
        $this->escopoQDCO = $escopo;
        
        if($this->escopoQDCO){
        	$this->escopo = 'QDCO';
        } else {
        	if($flgConstrutoraVinculada){
        		$this->escopo = 'QDCOV';
        	} else {
        		$this->escopo = 'QDCOC';
        	}
        }

        $this->obrid = $obra['obrid'];

        $questao = new Questao();
        $questionario = $questao->pegaTodaEstruturaCumprimentoObjeto($obra['codid'], $this->escopo);
        $respostas = $this->populaRespostas($questionario);
        return $respostas;
    }

    public function populaRespostas($questionario)
    {
        $obra = new Obras();
        $obra->carregarPorId($this->obrid);
        $fracasso = 0;
        foreach($questionario as $questao) {
            $questaoCumprimento = new QuestaoCumprimentoObjeto();
            $saida = $questaoCumprimento->pegaResposta($obra, $questao);
            if($saida['numero'] == 6) {
                if($saida['resposta'] == 't') {
                    $fracasso++;
                }
                continue;
            }
            if($saida['resposta'] == 'f') {
                $fracasso++;
            }
        }
        return $fracasso;
    }

    public function populaQuestionario($questionario, $flgConstrutoraExtra=false, $iDConstrutoraExtra = null)
    {
        $obra = new Obras();
        $obra->carregarPorId($this->obrid);
        $this->questoes = array();
        foreach($questionario as $questao) {
            $questao["construtoraextra"] = $flgConstrutoraExtra;
            $questaoCumprimento = new QuestaoCumprimentoObjeto();
            $questaoCumprimento->trataQuestao($obra, $questao, $iDConstrutoraExtra);
            $this->questoes[] = $questaoCumprimento;
        }
    }

    public function populaDescricao($questionario, $flgConstrutoraExtra=false)
    {
        $descricao = $questionario[0]['itcdsc'];
        if(!$this->escopoQDCO) {
            if ($flgConstrutoraExtra) {
                $descricao .= ' ('.mascaraglobal($this->obra['cexnumcnpj'],"##.###.###/####-##").') '.$this->obra['cexrazsocialconstrutora'];
            } else {
                $descricao .= ' ('.mascaraglobal($this->obra['entnumcpfcnpj'],"##.###.###/####-##").') '.$this->obra['entnome'];
            }
        }
        $this->descricaoItem = $descricao;
    }

    public function populaITCID($questionario)
    {
        $this->itcid = $questionario[0]['itcid'];
    }

    public function verificaExistencia()
    {
        $query = <<<DML
            SELECT qcodid FROM {$this->stNomeTabela} WHERE cumid = {$this->arAtributos['cumid']} AND qstid = {$this->arAtributos['qstid']} AND qcodstatus = 'A';
DML;
        return $this->pegaUm($query);
    }

    public function inativaResposta($qcodid)
    {
        $query = <<<DML
            UPDATE {$this->stNomeTabela} SET qcodresposta = 'f' WHERE qcodid = $qcodid;
DML;
        $this->executar($query);
        $this->commit();
    }

    public function devolverGru($arrDevolucao){
        $devvalor = $arrDevolucao['devvalor'];
        $codid  = $arrDevolucao['codid'];
        $obrid  = $arrDevolucao['obrid'];

        if($devvalor == null){
          return false;
        }

        $sqlBuscaDevolucao = "SELECT devid FROM obras2.devolucao_gru WHERE codid = {$codid} AND obrid = {$obrid}";
        $resultBuscaDevolucao = $this->pegaUm($sqlBuscaDevolucao);

        if(empty($resultBuscaDevolucao) && $devvalor != 0.00){
            $sql = "INSERT INTO obras2.devolucao_gru (codid,devvalor,obrid)VALUES({$codid},{$devvalor},{$obrid})";

        }else if  (!empty($resultBuscaDevolucao)){
            $sql = "UPDATE obras2.devolucao_gru SET devvalor = {$devvalor}  WHERE codid = {$codid} AND obrid = {$obrid}";
        }else{
            return false;
        }

        $this->executar($sql);
        $this->commit();

    }

    public function recuperarGruDevolvida($obrid){
        if($obrid == null){
            return false;
        }
        $sql="SELECT  COALESCE (devvalor, '0' ) AS devvalor  FROM obras2.devolucao_gru WHERE obrid = $obrid";

        return $this->pegaLinha($sql);

    }

}