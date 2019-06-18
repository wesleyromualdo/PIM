<?php
/**
 * Classe de mapeamento da entidade par3.obra_estudo_demanda.
 *
 * @version $Id$
 * @since 2017.08.24
 */

/**
 * Model_Obra_estudo_demanda: sem descricao
 *
 * @package Model\Obra
 * @uses Simec\Db\Modelo
 * @author Daniel Fiuza <danielfiuza@mec.gov.mec>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Model_Obra_estudo_demanda($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Model_Obra_estudo_demanda($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property string $oedexpansao_justificativa
 * @property string $oedremanejamento_justificativa
 * @property string $oedapliacao_justificativa
 * @property string $oedquadra
 * @property string $oeddemanda_justificativa
 * @property int $oed3_ano_qtd
 * @property bool $oed3_ano
 * @property int $oed2_ano_qtd
 * @property bool $oed2_ano
 * @property int $oed1_ano_qtd
 * @property bool $oed1_ano
 * @property int $oed9_serie_qtd
 * @property bool $oed9_serie
 * @property int $oed8_serie_qtd
 * @property bool $oed8_serie
 * @property int $oed7_serie_qtd
 * @property bool $oed7_serie
 * @property int $oed6_serie_qtd
 * @property bool $oed6_serie
 * @property int $oed5_serie_qtd
 * @property bool $oed5_serie
 * @property int $oed4_serie_qtd
 * @property bool $oed4_serie
 * @property int $oed3_serie_qtd
 * @property bool $oed3_serie
 * @property int $oed2_serie_qtd
 * @property bool $oed2_serie
 * @property int $oed1_serie_qtd
 * @property bool $oed1_serie
 * @property int $oedpre_escola_qtd
 * @property bool $oedpre_escola
 * @property int $oedcreche_3_qtd
 * @property bool $oedcreche_3
 * @property int $oedcreche_2_qtd
 * @property bool $oedcreche_2
 * @property int $oedcreche_1_qtd
 * @property bool $oedcreche_1
 * @property string $oedtipoampliacao_outros
 * @property string $oedmodalidade_noturno_outros
 * @property int $oedmodalidade_noturno
 * @property bool $oednoturno
 * @property string $oedturnos_atendimento
 * @property int $oedqtd_salas
 * @property int $omoid
 * @property int $obrid
 * @property int $oedid  - default: nextval('par3.obra_estudo_demanda_oedid_seq'::regclass)
 */
class Par3_Model_ObraEstudoDemanda extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.obra_estudo_demanda';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'oedid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'omoid' => array('tabela' => 'par3.obra_estudo_demanda_motivo', 'pk' => 'omoid'),
        'escid' => array('tabela' => 'par3.escola', 'pk' => 'escid'),
        'obrid' => array('tabela' => 'par3.obra', 'pk' => 'obrid'),
        'oemid' => array('tabela' => 'par3.obra_estudo_demanda_modalidade','pk' => 'oemid'),
        'etaid' => array('tabela' => 'par3.ensino_etapa','pk' => 'etaid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'oedexpansao_justificativa' => null,
        'oedremanejamento_justificativa' => null,
        'oedapliacao_justificativa' => null,
        'oedquadra' => null,
        'oeddemanda_justificativa' => null,
        'oed3_ano_qtd' => null,
        'oed3_ano' => null,
        'oed2_ano_qtd' => null,
        'oed2_ano' => null,
        'oed1_ano_qtd' => null,
        'oed1_ano' => null,
        'oed9_serie_qtd' => null,
        'oed9_serie' => null,
        'oed8_serie_qtd' => null,
        'oed8_serie' => null,
        'oed7_serie_qtd' => null,
        'oed7_serie' => null,
        'oed6_serie_qtd' => null,
        'oed6_serie' => null,
        'oed5_serie_qtd' => null,
        'oed5_serie' => null,
        'oed4_serie_qtd' => null,
        'oed4_serie' => null,
        'oed3_serie_qtd' => null,
        'oed3_serie' => null,
        'oed2_serie_qtd' => null,
        'oed2_serie' => null,
        'oed1_serie_qtd' => null,
        'oed1_serie' => null,
        'oedpre_escola_qtd' => null,
        'oedpre_escola' => null,
        'oedcreche_3_qtd' => null,
        'oedcreche_3' => null,
        'oedcreche_2_qtd' => null,
        'oedcreche_2' => null,
        'oedcreche_1_qtd' => null,
        'oedcreche_1' => null,
        'oedtipoampliacao_outros' => null,
        'oedtipoampliacao_outros_dsc' => null,
        'oedmodalidade_noturno_outros' => null,
        'oedmodalidade_noturno' => null,
        'oednoturno' => null,
        'oedturnos_atendimento' => null,
        'oedqtd_salas' => null,
        'omoid' => null,
        'obrid' => null,
        'oedid' => null,
        'oemid' => null,
        'etaid' => null,
        'oedexpansao_justificativa_dsc' => null,
        'oedremanejamento_justificativa_dsc' => null,
    );

    /**
     * Validators.
     *
     * @param mixed[] $dados
     * @return mixed[]
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'oedexpansao_justificativa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'oedremanejamento_justificativa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'oedapliacao_justificativa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'oedquadra' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'oeddemanda_justificativa' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 5000))),
            'oed3_ano_qtd' => array('allowEmpty' => true,'Digits'),
            'oed3_ano' => array('allowEmpty' => true),
            'oed2_ano_qtd' => array('allowEmpty' => true,'Digits'),
            'oed2_ano' => array('allowEmpty' => true),
            'oed1_ano_qtd' => array('allowEmpty' => true,'Digits'),
            'oed1_ano' => array('allowEmpty' => true),
            'oed9_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed9_serie' => array('allowEmpty' => true),
            'oed8_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed8_serie' => array('allowEmpty' => true),
            'oed7_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed7_serie' => array('allowEmpty' => true),
            'oed6_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed6_serie' => array('allowEmpty' => true),
            'oed5_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed5_serie' => array('allowEmpty' => true),
            'oed4_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed4_serie' => array('allowEmpty' => true),
            'oed3_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed3_serie' => array('allowEmpty' => true),
            'oed2_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed2_serie' => array('allowEmpty' => true),
            'oed1_serie_qtd' => array('allowEmpty' => true,'Digits'),
            'oed1_serie' => array('allowEmpty' => true),
            'oedpre_escola_qtd' => array('allowEmpty' => true,'Digits'),
            'oedpre_escola' => array('allowEmpty' => true),
            'oedcreche_3_qtd' => array('allowEmpty' => true,'Digits'),
            'oedcreche_3' => array('allowEmpty' => true),
            'oedcreche_2_qtd' => array('allowEmpty' => true,'Digits'),
            'oedcreche_2' => array('allowEmpty' => true),
            'oedcreche_1_qtd' => array('allowEmpty' => true,'Digits'),
            'oedcreche_1' => array('allowEmpty' => true),
            'oedtipoampliacao_outros' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oedmodalidade_noturno_outros' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 255))),
            'oedmodalidade_noturno' => array('allowEmpty' => true,'Digits'),
            'oednoturno' => array('allowEmpty' => true),
            'oedturnos_atendimento' => array('allowEmpty' => true,new Zend_Validate_StringLength(array('max' => 1))),
            'oedqtd_salas' => array('allowEmpty' => true,'Digits'),
            'omoid' => array('Digits'),
            'escid' => array('Digits'),
            'obrid' => array('Digits'),
        );
    }

    /**
     * Método de transformação de valores e validações adicionais de dados.
     *
     * Este método tem as seguintes finalidades:
     * a) Transformação de dados, ou seja, alterar formatos, remover máscaras e etc
     * b) A segunda, é a validação adicional de dados. Se a validação falhar, retorne false, se não falhar retorne true.
     *
     * @return bool
     */
    public function recuperarPorObra($obrid)
    {
        return $this->recuperar("SELECT  * FROM {$this->stNomeTabela} WHERE obrid = {$obrid}");
    }

    public function recuperarCategoriaObra($obrid)
    {
        return $this->recuperar(" select oct.octid  from par3.obra obr
                                     inner join par3.obra_tipo obrtp on obrtp.otpid = obr.otpid
                                     inner join par3.obra_categoria  oct on oct.octid = obrtp.octid
                                     WHERE obr.obrid  = {$obrid}");
    }
    public function antesSalvar()
    {
        // -- Implemente suas transformações de dados aqui

        // -- Por padrão, o método sempre retorna true
        return parent::antesSalvar();
    }

    public function calcularQtdAlunosAtendidos($oedid)
    {
        $mOed = new $this($oedid);
        $total = 0;
        if ($mOed->etaid == 1) {
            $total = ($mOed->oedcreche_1_qtd * 10)+
                ($mOed->oedcreche_2_qtd   * 16)+
                ($mOed->oedcreche_3_qtd   * 20)+
                ($mOed->oedpre_escola_qtd * 24);
        }
        if ($mOed->etaid == 2) {
            $total = ($mOed->oed1_serie_qtd * 36)+
                ($mOed->oed2_serie_qtd * 36)+
                ($mOed->oed3_serie_qtd * 36)+
                ($mOed->oed4_serie_qtd * 36)+
                ($mOed->oed5_serie_qtd * 36)+
                ($mOed->oed6_serie_qtd * 36)+
                ($mOed->oed7_serie_qtd * 36)+
                ($mOed->oed8_serie_qtd * 36)+
                ($mOed->oed9_serie_qtd * 36);
        }
        if ($mOed->etaid == 3) {
            $total = ($mOed->oed1_ano_qtd * 36)+
                ($mOed->oed2_ano_qtd * 36)+
                ($mOed->oed3_ano_qtd * 36);
        }

        if($mOed->oedturnos_atendimento == 'D'){
            return $total * 2;
        }
        return $total;
    }

    /**listarTiposObra
     * @param $qtdAlunos int
     * @param $esfera char(1)
     * @param $octid int
     * @return array|mixed|NULL|string
     * filtrar escola com alunos atendidos mais próximo
     */
    public function listarTiposObra($qtdAlunos, $esfera, $octid,$inpid,$tipoAmpliacao = '')
    {
        //TODO MOSTRAR MELHOR OPÇÃO DE TIPO DE OBRA DE ACORDO COM O QUE FOI CALCULADO PARA ALUNOS A SEREM ATENDIDOS
        $alunos = "AND otp.otpalunosatendidos > {$qtdAlunos}";

        $otptipoampliacao = '';
        if($tipoAmpliacao){
            switch($tipoAmpliacao){
                case 'Q':
                    $otptipoampliacao = 'AND otp.otptipoampliacao = '.Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_QUADRA;
                    break;
                case 'C':
                    $otptipoampliacao = 'AND otp.otptipoampliacao = '.Par3_Model_ObraCategoria::CATEGORIA_AMPLIACAO_COBERTURA;
                    break;
                default:
                    $otptipoampliacao = '';
                    break;
            }
        }
        $sql = "SELECT
                    otp.otpid as codigo, otp.otpid||' - '||otpdsc as descricao
                    FROM par3.obra_tipo otp
                    INNER JOIN par3.obra_categoria oct          on oct.octid = otp.octid
                    INNER JOIN par3.iniciativa_tipo_obra ini    on ini.otpid = otp.otpid
                    INNER JOIN par3.iniciativa_planejamento inp on inp.iniid = ini.iniid
                    INNER JOIN PAR3.obra_tipo_ensino_etapa ote  on ote.otpid = otp.otpid AND ote.etaid = inp.etaid
                    WHERE inp.inpid = {$inpid}
                    AND otp.otpesfera   in ('{$esfera}','T')
                    AND otp.octid       = {$octid}
                    AND otp.otpstatus   = 'A'
                    AND otp.otpsituacao = 'A'
                    {$otptipoampliacao}
                    ";
        $rsTipoObra = $this->carregar($sql);
        return is_array($rsTipoObra)?$rsTipoObra:array();
    }
}
