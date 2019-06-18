<?php
/**
 * Classe de mapeamento da entidade par3.projeto_historico.
 *
 * @version $Id$
 * @since 2017.04.17
 */

/**
 * Par3_Model_Projeto_historico: sem descricao
 *
 * @package Par3\Model
 * @uses Simec\Db\Modelo
 * @author Daniel Da Rocha Fiuza <danielfiuza@mec.gov.br>
 *
 * @example
 * <code>
 * // -- Consultando registros
 * $model = new Par3_Model_Projeto_historico($valorID);
 * var_dump($model->getDados());
 *
 * // -- Alterando registros
 * $valores = ['campo' => 'valor'];
 * $model = new Par3_Model_Projeto_historico($valorID);
 * $model->popularDadosObjeto($valores);
 * $model->salvar(); // -- retorna true ou false
 * $model->commit();
 * </code>
 *
 * @property  $hid chave primária da tabelaprojeto_historico
 * @property  $hcpf CPF do respnsável pela alteração
 * @property  $hacao Ações de Inserção UPDATE,INSERT,DELETE
 * @property  $hdtcriacao Data de criação do log
 * @property int $proid Chave Primária - default: nextval('par3.projeto_proid_seq'::regclass)
 * @property string $prosigla sigla do projeto
 * @property string $prodsc Descrição do projeto
 * @property string $prolink Link do projeto
 * @property string $proresumo resumo do projeto
 * @property "char" $prostatus status do projeto: indidica se o projeto foi removido
 * @property "char" $prosituacao situação do projeto: indidica se o projeto está ativo/inativo
 * @property \Datetime(Y-m-d H:i:s) $prodtinclusao data de inclusão do projeto
 * @property string $procpfinclusao cpf do responsável pela inclusão do projeto
 * @property \Datetime(Y-m-d H:i:s) $prodtinativacao data de inativação do projeto
 * @property string $procpfinativacao cpf do responsável pela inativação do projeto
 */
class Par3_Model_ProjetoHistorico extends Modelo
{
    /**
     * @var string Nome da tabela mapeada.
     */
    protected $stNomeTabela = 'par3.projeto_historico';

    /**
     * @var string[] Chave primaria.
     */
    protected $arChavePrimaria = array(
        'hprid',
    );

    /**
     * @var mixed[] Chaves estrangeiras.
     */
    protected $arChaveEstrangeira = array(
        'proid' => array('tabela' => 'par3.projeto', 'pk' => 'proid'),
    );

    /**
     * @var mixed[] Atributos da tabela.
     */
    protected $arAtributos = array(
        'hprid' => null,
        'hprcpf' => null,
        'hpracao' => null,
        'hprdtcriacao' => null,
        'proid' => null,
        'prosigla' => null,
        'prodsc' => null,
        'prolink' => null,
        'proresumo' => null,
        'prostatus' => null,
        'prosituacao' => null,
        'prodtinclusao' => null,
        'procpfinclusao' => null,
        'prodtinativacao' => null,
        'procpfinativacao' => null,
    );


            //flags de ação
    const CREATE = 'CREATE';
    const UPDATE = 'UPDATE';
    const DELETE = 'DELETE';

    public function gravarHistorico(Par3_Model_Projeto $model,$acao)
    {
        $this->arAtributos['hprcpf'] = $_SESSION['usucpf'];
        $this->arAtributos['hpracao'] = $acao;
        $this->arAtributos['hprdtcriacao'] = date('Y-m-d H:m:s');
        $this->arAtributos['proid'] = $model->proid;
        $this->arAtributos['prosigla'] = $model->prosigla;
        $this->arAtributos['prodsc'] = $model->prodsc;
        $this->arAtributos['prolink'] = $model->prolink;
        $this->arAtributos['proresumo'] = $model->proresumo;
        $this->arAtributos['prostatus'] = $model->prostatus;
        $this->arAtributos['prosituacao'] = $model->prosituacao;
        $this->arAtributos['prodtinclusao'] = $model->prodtinclusao;
        $this->arAtributos['procpfinclusao'] = $model->procpfinclusao;
        $this->arAtributos['prodtinativacao'] = $model->prodtinativacao;
        $this->arAtributos['procpfinativacao'] = $model->procpfinativacao;
        //grava no histórico
        $this->popularDadosObjeto($this->arAtributos);
        $this->salvar();
        $this->commit();
    }
}
