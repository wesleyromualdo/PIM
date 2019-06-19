<?php
/**
 * Classe de mapeamento da entidade par3.iniciativa_programas_termo_adesao_historico.
 *
 * @version $Id$
 * @since 2017.04.26
 */

/**
 * Par3_Model_Iniciativa_programas_termo_adesaoHistorico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Iniciativa_programas_termo_adesaoHistorico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Iniciativa_programas_termo_adesaoHistorico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hteid chave primária da tabelainiciativa_programas_termo_adesao_historico
 * @property  $htecpf CPF do respnsável pela alteração
 * @property  $hteacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $htedtcriacao Data de criação do log
 * @property int $terid  - default: nextval('par3.iniciativa_programas_termo_adesao_terid_seq'::regclass)
 * @property int $inprgid 
 * @property int $tpdid 
 * @property string $terpacto 
 * @property string $tertermo 
 * @property string $teraceitepacto 
 * @property string $ternaoaceitepacto 
 * @property string $teraceitetermo 
 * @property string $ternaoaceitetermo 
 * @property string $teremailpreanalise 
 * @property int    $teranoreferencia
 * @property string $tercabecalhoadesao 
 * @property string $terinstrucoes 
 * @property string $termsgbemvindo 
 * @property string $terredirecionamentoadesao
 * @property string $terdtinclusao
 * @property string $terdtinativacao
 * @property string $tercpfinativacao
 * @property string $terstatus
 * @property string $tersituacao
 */
class Par3_Model_ProAdesaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.proadesao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hpaid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'pfaid'   => array('tabela' => 'par3.proadesao', 'pk' => 'pfaid'),
        'prgid' => array('tabela' => 'par3.programa', 'pk' => 'prgid'),

    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hpaid'                 => null,
        'hpacpf'                => null,
        'hpaacao'               => null,
        'hpadtcriacao'          => null,
        'pfaid'                 => null,
        'prgid'                 => null,
        'pfadatainicial'        => null,
        'pfadatafinal'          => null,
        'pfaano'                => null,
        'pfaicone'              => null,
        'pfaesfera'             => null,
        'pfastatus'             => null,
        'pfatermomunicipio'     => null,
        'pfamsgboasvindas'      => null,
        'pfamsgdeaceitacao'     => null,
        'pfamsgdenaoaceitacao'  => null,
        'pfatermoestado'        => null,
        'pfabackground'         => null,
        'pfafundocor'           => null,
        'pfasobre'              => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Proadesao $model,$acao)
    {
    
        $this->arAtributos['hpacpf']                = $_SESSION['usucpf'];
        $this->arAtributos['hpaacao']               = $acao;
        $this->arAtributos['hpadtcriacao']          = date('Y-m-d H:m:s');
        $this->arAtributos['pfaid']                 = $model->pfaid;
        $this->arAtributos['prgid']                 = $model->prgid;
        $this->arAtributos['pfadatainicial']        = $model->pfadatainicial;
        $this->arAtributos['pfadatafinal']          = $model->pfadatafinal;
        $this->arAtributos['pfaano']                = $model->pfaano;
        $this->arAtributos['pfaicone']              = $model->pfaicone;
        $this->arAtributos['pfaesfera']             = $model->pfaesfera;
        $this->arAtributos['pfastatus']             = $model->pfastatus;
        $this->arAtributos['pfatermomunicipio']     = $model->pfatermomunicipio;
        $this->arAtributos['pfamsgboasvindas']      = $model->pfamsgboasvindas;
        $this->arAtributos['pfamsgdeaceitacao']     = $model->pfamsgdeaceitacao;
        $this->arAtributos['pfamsgdenaoaceitacao']  = $model->pfamsgdenaoaceitacao;
        $this->arAtributos['pfatermoestado']        = $model->pfatermoestado;
        $this->arAtributos['pfabackground']         = $model->pfabackground;
        $this->arAtributos['pfafundocor']           = $model->pfafundocor;
        $this->arAtributos['pfasobre']              = $model->pfasobre;

        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
