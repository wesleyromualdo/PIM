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
class Par3_Model_IniciativaProgramasTermoAdesaoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.iniciativa_programas_termo_adesao_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hteid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'terid'   => array('tabela' => 'par3.iniciativa_programas_termo_adesao', 'pk' => 'terid'),
        'inprgid' => array('tabela' => 'par3.iniciativa_programas', 'pk' => 'inprgid'),

    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hteid'                     => null,
        'htecpf'                    => null,
        'hteacao'                   => null,
        'htedtcriacao'              => null,
        'terid'                     => null,
        'inprgid'                   => null,
        'tpdid'                     => null,
        'terpacto'                  => null,
        'tertermo'                  => null,
        'teraceitepacto'            => null,
        'ternaoaceitepacto'         => null,
        'teraceitetermo'            => null,
        'ternaoaceitetermo'         => null,
        'teremailpreanalise'        => null,
        'teranoreferencia'          => null,
        'tercabecalhoadesao'        => null,
        'terinstrucoes'             => null,
        'termsgbemvindo'            => null,
        'terredirecionamentoadesao' => null,
        'terdtinclusao'             => null,
        'tercpfinclusao'            => null,
        'terdtinativacao'           => null,
        'tercpfinativacao'          => null,
        'terstatus'                 => null,
        'tersituacao'               => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_IniciativaProgramasTermoAdesao $model,$acao)
    {
    
        $this->arAtributos['htecpf']                    = $_SESSION['usucpf'];
        $this->arAtributos['hteacao']                   = $acao;
        $this->arAtributos['htedtcriacao']              = date('Y-m-d H:m:s');
        $this->arAtributos['terid']                     = $model->terid;
        $this->arAtributos['inprgid']                   = $model->inprgid;
        $this->arAtributos['tpdid']                     = $model->tpdid;
        $this->arAtributos['terpacto']                  = $model->terpacto;
        $this->arAtributos['tertermo']                  = $model->tertermo;
        $this->arAtributos['teraceitepacto']            = $model->teraceitepacto;
        $this->arAtributos['ternaoaceitepacto']         = $model->ternaoaceitepacto;
        $this->arAtributos['teraceitetermo']            = $model->teraceitetermo;
        $this->arAtributos['ternaoaceitetermo']         = $model->ternaoaceitetermo;
        $this->arAtributos['teremailpreanalise']        = $model->teremailpreanalise;
        $this->arAtributos['teranoreferencia']          = $model->teranoreferencia;
        $this->arAtributos['tercabecalhoadesao']        = $model->tercabecalhoadesao;
        $this->arAtributos['terinstrucoes']             = $model->terinstrucoes;
        $this->arAtributos['termsgbemvindo']            = $model->termsgbemvindo;
        $this->arAtributos['terredirecionamentoadesao'] = $model->terredirecionamentoadesao;
        $this->arAtributos['terstatus']                 = $model->terstatus;
        $this->arAtributos['teresfera']                 = $model->teresfera;
        $this->arAtributos['tersituacao']               = $model->tersituacao;
        $this->arAtributos['terdtinclusao']             = $model->terdtinclusao;
        $this->arAtributos['tercpfinclusao']            = $model->tercpfinclusao;
        $this->arAtributos['terdtinativacao']           = $model->terdtinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
